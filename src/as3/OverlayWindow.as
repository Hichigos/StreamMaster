package
{
	/**
	 * ...
	 * @author Butsianouski Denis
	 */
	
	import flash.display.Bitmap;
	import flash.events.Event;
	import flash.events.FullScreenEvent;
	import mx.core.BitmapAsset;
	import net.wg.infrastructure.base.AbstractScreen;
	import net.wg.gui.components.controls.SoundButton;
	import net.wg.gui.components.controls.TextInput;
	import flash.text.*;
	import scaleform.clik.events.ButtonEvent;
	
	public class OverlayWindow extends AbstractScreen
	{
		private var youTubeBtn: SoundButton;
		private var twitchBtn: SoundButton;
		private var toggleStreamBtn: SoundButton;
		private var quitBtn: SoundButton;
		
		[Embed(source= "res/blackscreen.jpg")]
		private var BImage: Class;
		private var background: BitmapAsset;

		private var tokenInput: TextInput;
		private var statusText: TextField;
		private var statusLabel: TextField;
		private var titleLabel: TextField;
		
		public var onToggleStream: Function = null;
		public var onWindowSizeChanged: Function = null;
		public var onServiceChanged: Function = null;
		public var onWindowClose: Function = null;
		public var checkIfTokenValid: Function = null;
		public var log: Function = null;
		
		public var getStreamState: Function = null;
		public var getStreamService: Function = null;
		public var getStreamToken: Function = null;
		public var getClientWindowWidth: Function = null;
		public var getClientWindowHeight: Function = null;
		
		public function OverlayWindow() {
			super();
		}
		
		override protected function onPopulate(): void {
			super.onPopulate();	
			
			background = new BImage() as BitmapAsset;
			background.alpha = 0.4;
			stage.addChild(background);

			youTubeBtn = addChild(App.utils.classFactory.getComponent("ButtonRed", SoundButton, {
				width: 100, 
				height: 80, 
				x: 172, 
				y: 50, 
				label: "YouTube", 
				enabled: true, 
				alpha: 0.5
			})) as SoundButton;
			
			youTubeBtn.addEventListener(ButtonEvent.CLICK, this.onYouTubeBtnClicked);
			
			twitchBtn = addChild(App.utils.classFactory.getComponent("ButtonRed", SoundButton, {
				width: 100,
				height: 80,
				x: 172,
				y: 50,
				label: "Twitch",
				enabled: true,
				alpha: 0.5
			})) as SoundButton;
			
			twitchBtn.addEventListener(ButtonEvent.CLICK, this.onTwitchBtnClicked);
			
			toggleStreamBtn = addChild(App.utils.classFactory.getComponent("ButtonRed", SoundButton, {
				width: 100,
				height: 25,
				x: 172,
				y: 50,
				label: "Start",
				enabled: false
			})) as SoundButton;
			
			toggleStreamBtn.addEventListener(ButtonEvent.CLICK, this.onToggleStreamClicked);
			
			quitBtn = addChild(App.utils.classFactory.getComponent("CloseButton", SoundButton, {
				width: 25,
				height: 25,
				x: 1,
				y: 1,
				label: "",
				enabled: true
			})) as SoundButton;
			
			quitBtn.addEventListener(ButtonEvent.CLICK, this.onCloseBtnClicked);
			
			tokenInput = addChild(App.utils.classFactory.getComponent("TextInput", TextInput, {
				width: 152,
				height: 29,
				x: 10,
				y: 48,
				enabled: true
			})) as TextInput;
			
			tokenInput.addEventListener(Event.CHANGE, this.onTokenChanged);
			
			statusText = new TextField();
            statusText.multiline = false;
            statusText.selectable = false;
			statusText.visible = false;
            statusText.defaultTextFormat = new TextFormat("$FieldFont", 14, 0xd80d0d);
            statusText.text = "";
            addChild(statusText);

			statusLabel = new TextField();
            statusLabel.multiline = false;
            statusLabel.selectable = false;
			statusLabel.visible = false;
            statusLabel.defaultTextFormat = new TextFormat("$FieldFont", 14, 0xd80d0d);
            statusLabel.text = "Status: ";
            addChild(statusLabel);

			titleLabel = new TextField();
            titleLabel.multiline = false;
            titleLabel.selectable = false;
            titleLabel.defaultTextFormat = new TextFormat("$FieldFont", 18, 0xffffff);
            titleLabel.text = "Stream Master";
			titleLabel.width = titleLabel.textWidth + 5;
            addChild(titleLabel);

			addEventListener(Event.RESIZE, onResizeEvent);
			addEventListener(Event.FULLSCREEN, onResizeEvent);

			updateControls();
		}
		
		override protected function draw() : void {
			this.height = getClientWindowHeight();
			this.width = getClientWindowWidth();
			
			var dx:Number = this.width / 2;
			var dy:Number = this.height / 2;
			
			youTubeBtn.x = dx - youTubeBtn.width - youTubeBtn.width / 2;
			youTubeBtn.y = dy - youTubeBtn.height;
			
			twitchBtn.x = dx + twitchBtn.width + twitchBtn.width / 2;
			twitchBtn.y = dy - twitchBtn.height;
			
			toggleStreamBtn.x = youTubeBtn.x;
			toggleStreamBtn.y = youTubeBtn.y + youTubeBtn.height + 5;
			
			tokenInput.x = toggleStreamBtn.x + toggleStreamBtn.width + 5;
			tokenInput.y = toggleStreamBtn.y;
			tokenInput.width = twitchBtn.x + twitchBtn.width - tokenInput.x;
			
			quitBtn.x = this.width - quitBtn.width - 5;
			
			statusLabel.x = toggleStreamBtn.x;
			statusLabel.y = toggleStreamBtn.y + toggleStreamBtn.height + 5;

			statusText.x = statusLabel.x + statusLabel.width + 5;
			statusText.y = statusLabel.y;

			titleLabel.x = dx - titleLabel.textWidth / 2;
			titleLabel.y = 1;

			log("draw");
			
			super.draw();
		}
		
		override protected function onEscapeKeyDown() : void {
			stage.removeChild(background);
			this.onWindowClose();
			this.dispose();
		}
		
		public function updateControls(): void {
			
			var isRunning: Boolean = (this.getStreamState() == "strd");

			twitchBtn.enabled = !isRunning;
			youTubeBtn.enabled = !isRunning;
			tokenInput.enabled = !isRunning;
			toggleStreamBtn.label = isRunning ? "Stop" : "Start";
			tokenInput.text = isRunning ? "token" : getStreamToken();
			toggleStreamBtn.enabled = isRunning;
			
			log("Is Running: " + isRunning.toString());
			
			if (getStreamService() == "YouTube") {
				youTubeBtn.alpha = 1;
				twitchBtn.alpha = 0.5;
			} else if (getStreamService() == "Twitch") {
				twitchBtn.alpha = 1;
				youTubeBtn.alpha = 0.5;
			}
			
			//setToken(getStreamToken());
		}
		
		public function token(): String {
			return tokenInput.text;
		}
		
		public function setToken(text: String): void {
			tokenInput.text = text;
			this.toggleStreamBtn.enabled = this.checkIfTokenValid();
		}
		
		public function setStatusText(text: String): void {
			statusText.visible = text.length > 0;
			statusLabel.visible = text.length > 0;
			statusText.text = text;		
		}
		
		private function onYouTubeBtnClicked(param:ButtonEvent): void {
			onServiceChanged("YouTube");
		}
		
		private function onTwitchBtnClicked(param:ButtonEvent): void {
			onServiceChanged("Twitch");
		}
		
		private function onToggleStreamClicked(param:ButtonEvent): void {
			onToggleStream();
		}
		
		private function onCloseBtnClicked(param: ButtonEvent): void {
			stage.removeChild(background);
			this.onWindowClose();
			this.dispose();
		}
		
		private function onTokenChanged(param: Event): void {
			this.toggleStreamBtn.enabled = this.checkIfTokenValid();
		}
		
		private function onResizeEvent(param: Event): void {
			onWindowSizeChanged();
		}
	}

}