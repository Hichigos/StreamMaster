package 
{
	/**
	 * ...
	 * @author hichigos
	 */
	
	import flash.display.DisplayObject;
    import flash.display.SimpleButton;
    import flash.display.Sprite;
	
	import ButtonImageState;
	 
	public class ServiceButton 
	{
		
		public function ServiceButton (upState : DisplayObject = null,
		overState : DisplayObject = null,
		downState : DisplayObject = null,
		hitTestState : DisplayObject = null) {
			
			upState = new ButtonImageState( upAlpha);
            overState = new ButtonImageState( overAlpha);
            downState = new ButtonImageState( upAlpha);
            hitTestState = new ButtonImageState( upAlpha);

            super(upState, overState, downState, hitTestState);
		}
		
	}

}