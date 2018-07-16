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
	import flash.events.MouseEvent;
	import ServiceButton;
	import HeaderSprite;
	
	public class OverlayWindow extends AbstractScreen
	{
		private var youTubeBtn: ServiceButton;
		private var twitchBtn: ServiceButton;
		private var facebookBtn: ServiceButton;
		
		private var toggleStreamBtn: SoundButton;
		private var quitBtn: SoundButton;
		
		[Embed(source= "res/blackscreen.jpg")]
		private var BImage: Class;
		private var background: BitmapAsset;

		private var tokenInput: TextInput;
		private var statusText: TextField;
		private var statusLabel: TextField;
		private var titleLabel: TextField;
		
		private var header : HeaderSprite;
		
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
			background.alpha = 0.7;
			addChild(background);
			
			header = new HeaderSprite();
			addChild(header);

			youTubeBtn = new ServiceButton("YouTube");
			youTubeBtn.width = 128;
			youTubeBtn.height = 128;
			youTubeBtn.enabled = true;
			addChild(youTubeBtn);
			
			youTubeBtn.addEventListener(MouseEvent.CLICK, onYouTubeBtnClicked);
			
			twitchBtn = new ServiceButton("Twitch");
			twitchBtn.width = 128;
			twitchBtn.height = 128;
			twitchBtn.enabled = true;
			addChild(twitchBtn);
			
			twitchBtn.addEventListener(MouseEvent.CLICK, onTwitchBtnClicked);
			
			facebookBtn = new ServiceButton("Facebook");
			facebookBtn.width = 128;
			facebookBtn.height = 128;
			facebookBtn.enabled = true;
			addChild(facebookBtn);
			
			facebookBtn.addEventListener(MouseEvent.CLICK, onFacebookBtnClicked);
			
			toggleStreamBtn = addChild(App.utils.classFactory.getComponent("ButtonRed", SoundButton, {
				width: 120,
				height: 25,
				x: 172,
				y: 50,
				label: "Start",
				enabled: false
			})) as SoundButton;
			
			toggleStreamBtn.addEventListener(ButtonEvent.CLICK, this.onToggleStreamClicked);
			
			quitBtn = addChild(App.utils.classFactory.getComponent("CloseButton", SoundButton, {
				width: 30,
				height: 30,
				x: 1,
				y: 1,
				label: "",
				enabled: true
			})) as SoundButton;
			
			quitBtn.addEventListener(ButtonEvent.CLICK, this.onCloseBtnClicked);
			
			tokenInput = addChild(App.utils.classFactory.getComponent("TextInput", TextInput, {
				width: 152,
				height: 25,
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
			var btnWidth:Number = 128;
			var gap:Number = 5;
			var btnsRowWidth:Number = (128 + 5) * 3;
				
			youTubeBtn.x = dx - btnsRowWidth / 2;
			youTubeBtn.y = dy - youTubeBtn.height;
			
			twitchBtn.x = youTubeBtn.x + youTubeBtn.width + gap;
			twitchBtn.y = dy - twitchBtn.height;
			
			facebookBtn.x = twitchBtn.x + twitchBtn.width + gap;
			facebookBtn.y = dy - facebookBtn.height;
			
			toggleStreamBtn.x = youTubeBtn.x;
			toggleStreamBtn.y = youTubeBtn.y + youTubeBtn.height + 10;
			
			tokenInput.x = toggleStreamBtn.x + toggleStreamBtn.width + 5;
			tokenInput.y = toggleStreamBtn.y;
			tokenInput.width = facebookBtn.x + facebookBtn.width - tokenInput.x;
			
			header.width = this.width;
			header.height = 37;
			
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
			this.onWindowClose();
			this.dispose();
		}
		
		public function updateControls(): void {
			
			var isRunning: Boolean = (this.getStreamState() == "strd");

			twitchBtn.enabled = !isRunning;
			youTubeBtn.enabled = !isRunning;
			facebookBtn.enabled = !isRunning;
			tokenInput.enabled = !isRunning;
			toggleStreamBtn.label = isRunning ? "Stop" : "Start";
			tokenInput.text = isRunning ? "token" : getStreamToken();
			toggleStreamBtn.enabled = isRunning;
			
			if (getStreamService() == "YouTube") {
				youTubeBtn.isChecked = true;
				twitchBtn.isChecked = false;
				facebookBtn.isChecked = false;
			} else if (getStreamService() == "Twitch") {
				youTubeBtn.isChecked = false;
				twitchBtn.isChecked = true;
				facebookBtn.isChecked = false;
			} else if (getStreamService() == "Facebook"){
				youTubeBtn.isChecked = false;
				twitchBtn.isChecked = false;
				facebookBtn.isChecked = true;
			}
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
		
		private function onYouTubeBtnClicked(param:MouseEvent): void {
			onServiceChanged("YouTube");
		}
		
		private function onTwitchBtnClicked(param:MouseEvent): void {
			onServiceChanged("Twitch");
		}
		
		private function onFacebookBtnClicked(param:MouseEvent): void {
			onServiceChanged("Facebook");
		}
		
		private function onToggleStreamClicked(param:ButtonEvent): void {
			onToggleStream();
		}
		
		private function onCloseBtnClicked(param: ButtonEvent): void {
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