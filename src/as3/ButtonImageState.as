package 
{
	/**
	 * ...
	 * @author hichigos
	 */
	
    import flash.display.Loader;
    import flash.display.Sprite;
    import flash.net.URLRequest;
	
	public class ButtonImageState extends Sprite{
		
		private var upAlpha : Number = 1;
        private var overAlpha : Number = 0.5;
		
        public function ButtonImageState(_alpha:Number, path: String)
        {
            var my_loader : Loader = new Loader();
            my_loader.load(new URLRequest("car.jpg"));
            addChild(my_loader);

            this.alpha = _alpha;
        }
		
	}

}