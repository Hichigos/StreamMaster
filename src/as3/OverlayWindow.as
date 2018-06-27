package
{
	/**
	 * ...
	 * @author Butsianouski Denis
	 */
	
	import flash.events.Event;
	import flash.events.FullScreenEvent;
	import net.wg.infrastructure.base.AbstractScreen;
	import net.wg.gui.components.controls.SoundButton;
	import net.wg.gui.components.controls.TextInput;
	import flash.text.*;
	import scaleform.clik.events.ButtonEvent;
	import net.wg.gui.components.common.containers.VerticalGroupLayout;
	
	public class OverlayWindow extends AbstractScreen
	{
		private var layout: VerticalGroupLayout;
		private var youTubeBtn: SoundButton;
		private var twitchBtn: SoundButton;
		private var toggleStreamBtn: SoundButton;
		private var quitBtn: SoundButton;
		
		private var tokenInput: TextInput;
		private var statusText: TextField;
		
		public var onToggleStream: Function = null;
		public var onServiceChanged: Function = null;
		public var onWindowClose: Function = null;
		public var checkIfTokenValid: Function = null;
		public var log: Function = null;
		
		public var getStreamState: Function = null;
		public var getClientWindowWidth: Function = null;
		public var getClientWindowHeight: Function = null;
		
		public function OverlayWindow() {
			super();
		}
		
		override protected function onPopulate(): void {
			super.onPopulate();	
			
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
			
			this.updateControls();
		}
		
		private function updateControls(): void {
			this.height = getClientWindowHeight();
			this.width = getClientWindowWidth();
			
			var dx:Number = this.width / 6;
			var dy:Number = this.height / 4;
			
			youTubeBtn.x = dx;
			youTubeBtn.y = dy;
			
			twitchBtn.x = youTubeBtn.x + youTubeBtn.width + dx;
			twitchBtn.y = dy;
			
			toggleStreamBtn.x = youTubeBtn.x + youTubeBtn.width + dx / 2;
			toggleStreamBtn.y = youTubeBtn.y + youTubeBtn.height + 5;
			
			tokenInput.x = toggleStreamBtn.x + toggleStreamBtn.width + 5;
			tokenInput.y = toggleStreamBtn.y;
			
			quitBtn.x = this.width - quitBtn.width - 5;
			
			statusText.x = toggleStreamBtn.x;
			statusText.y = toggleStreamBtn.y + toggleStreamBtn.height + 5;
			
			log(youTubeBtn.x.toString() + " " + youTubeBtn.y.toString());
			log(twitchBtn.x.toString() + " " + twitchBtn.y.toString());
			log(toggleStreamBtn.x.toString() + " " + toggleStreamBtn.y.toString());
		}
		
		public function service(): String {
			return twitchBtn.alpha == 1 ? "Twitch" : "YouTube";
		}
		
		public function token(): String {
			return tokenInput.text;
		}
		
		public function setStatusText(text: String): void {
			statusText.visible = text.length > 0;
			statusText.text = text;		
		}
		
		private function onYouTubeBtnClicked(param:ButtonEvent): void {
			youTubeBtn.alpha = 1;
			twitchBtn.alpha = 0.5;
			onServiceChanged();

			updateControls();			
		}
		
		private function onTwitchBtnClicked(param:ButtonEvent): void {
			twitchBtn.alpha = 1;
			youTubeBtn.alpha = 0.5;
			this.onServiceChanged();
			
			updateControls();
		}
		
		private function onToggleStreamClicked(param:ButtonEvent): void {
			this.onToggleStream();
			var isRunning: Boolean = (this.getStreamState() == 2);
			this.log("isRunning " + isRunning.toString());
			twitchBtn.enabled = isRunning;
			youTubeBtn.enabled = isRunning;
			toggleStreamBtn.label = isRunning ? "Stop" : "Start";
		}
		
		private function onCloseBtnClicked(param: ButtonEvent): void {
			this.onWindowClose();
			this.dispose();
		}
		
		private function onTokenChanged(param: Event): void {
			this.toggleStreamBtn.enabled = this.checkIfTokenValid();
		}
	
	}

}