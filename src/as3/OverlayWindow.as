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
	
	public class OverlayWindow extends AbstractScreen
	{
		private var youTubeBtn: SoundButton;
		private var twitchBtn: SoundButton;
		private var toggleStreamBtn: SoundButton;
		private var quitBtn: SoundButton;
		
		private var tokenInput: TextInput;
		
		public var toggleStream: Function = null;
		public var getClientWindowWidth: Function = null;
		public var getClientWindowHeight: Function = null;
		public var log: Function = null;
		
		public function OverlayWindow() {
			super();
		}
		
		override protected function onPopulate(): void {
			super.onPopulate();
			width = 400;
			height = 200;
			
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
			
			quitBtn = addChild(App.utils.classFactory.getComponent("ButtonRed", SoundButton, {
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
			
			/**tokenInput.addEventListener(Event.CHANGE, this.onTokenChanged);**/
			
			this.updateControls();
		}
		
		private function updateControls(): void {
			this.height = getClientWindowHeight();
			this.width = getClientWindowWidth();
			
			var dx:Number = this.width / 4;
			var dy:Number = this.height / 4;
			
			youTubeBtn.x = dx;
			youTubeBtn.y = dy;
			
			twitchBtn.x = youTubeBtn.x + youTubeBtn.width + dx;
			twitchBtn.y = dy;
			
			toggleStreamBtn.x = youTubeBtn.x + youTubeBtn.width + dx / 2;
			toggleStreamBtn.y = youTubeBtn.y + youTubeBtn.height + 5;
			
			tokenInput.x = toggleStreamBtn.x + toggleStreamBtn.width + 5;
			tokenInput.y = toggleStreamBtn.y;
			
			var isServiceSelected: Boolean = (youTubeBtn.alpha == 1 || twitchBtn.alpha == 1);
			
			toggleStreamBtn.enabled = isServiceSelected;
			tokenInput.visible = isServiceSelected;
			
			quitBtn.x = this.width - quitBtn.width - 5;
			
			log(youTubeBtn.x.toString() + " " + youTubeBtn.y.toString());
			log(twitchBtn.x.toString() + " " + twitchBtn.y.toString());
			log(toggleStreamBtn.x.toString() + " " + toggleStreamBtn.y.toString());
		}
		
		private function onYouTubeBtnClicked(param:ButtonEvent): void {
			youTubeBtn.alpha = 1;
			twitchBtn.alpha = 0.5;
			updateControls();			
		}
		
		private function onTwitchBtnClicked(param:ButtonEvent): void {
			twitchBtn.alpha = 1;
			youTubeBtn.alpha = 0.5;
			updateControls();
		}
		
		private function onToggleStreamClicked(param:ButtonEvent): void {
			this.toggleStream();
		}
		
		private function onCloseBtnClicked(param: ButtonEvent): void {
			this.dispose();
		}
	
	}

}