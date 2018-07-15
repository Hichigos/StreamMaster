package 
{
	/**
	 * ...
	 * @author Butsianouski Denis
	 */
	
	import flash.display.DisplayObject;
    import flash.display.SimpleButton;
	
	import ButtonImageState;
	 
	public class ServiceButton extends SimpleButton
	{
		private var upAlpha : Number = 0.6;
        private var overAlpha : Number = 1;
		
		private var m_isChecked : Boolean = false;
		
		public function ServiceButton (name : String) {
			
			upState = new ButtonImageState( upAlpha, name);
            overState = new ButtonImageState( overAlpha, name);
            downState = new ButtonImageState( upAlpha, name);
            hitTestState = new ButtonImageState( upAlpha, name);

            super(upState, overState, downState, hitTestState);
		}
		
		public function get isChecked() : Boolean {
			return m_isChecked;
		}
			
		public function set isChecked(_isChecked : Boolean) : void {
			upState.alpha = _isChecked ? overAlpha : upAlpha;
				
			m_isChecked = _isChecked;
		}
		
		
	}

}