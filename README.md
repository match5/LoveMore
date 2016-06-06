# LoveMore
Native extensions for LÖVE

​	***GameObject and Component***

	obj = lovemore.newGameObject();
	obj:addComponent(lovemore.newComponent(
	{
		name = "test",
	    update = function(self, dt)
			self.transform:rotate(dt * 90);
		end
	});
​	**SpriteRenderer**

	img = love.graphics.newImage("pic.png");
	
	sp = lovemore.newGameObject();
	com = lovemore.newSpriteRenderer(img);
	sp:addComponent(com);
	sp.transform:setPosition(100, 100);
	
	sp2 = lovemore.newSprite(img);
	sp2.transform:setPosition(50, 50);
	sp2.parent = sp;


​	**SpineAnimator**

	spine = lovemore.newGameObject();
	com = lovemore.newSpineAnimator("spineboy.json", "spineboy.atlas");
	com:setAnimation(0, "run", true);
	spine:addComponent(com);