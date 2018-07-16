package 
{
	/**
	 * ...
	 * @author hichigos
	 */
	
	import flash.display.Bitmap;
	import flash.display.Sprite;
	
	 
	public class HeaderSprite extends Sprite {
		
		[Embed(source= "res/blackscreen.jpg")]
		private var BImage: Class;
		
		public function HeaderSprite () {
			super();
			
			var background : Bitmap = new BImage() as Bitmap;
			background.alpha = 0.8;
			addChild(background);
		}
	}

}