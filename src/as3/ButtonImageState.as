package 
{
	/**
	 * ...
	 * @author hichigos
	 */
	
	import flash.display.Bitmap;
    import flash.display.Sprite;
	
	public class ButtonImageState extends Sprite {
		
		[Embed(source= "res/youtube.png")]
		private var YoutubePic: Class;
		
		[Embed(source= "res/twitch.png")]
		private var TwitchPic: Class;
		
		[Embed(source= "res/facebook.png")]
		private var FacebookPic: Class;
		
        public function ButtonImageState(_alpha:Number, name: String)
        {
			var pic : Bitmap = null;
			
			if (name == "YouTube") {
				pic = new YoutubePic() as Bitmap;
			} else if (name == "Twitch") {
				pic = new TwitchPic() as Bitmap;
			} else if (name == "Facebook") {
				pic = new FacebookPic() as Bitmap;
			}
			
			addChild(pic);

            this.alpha = _alpha;
        }		
	}

}