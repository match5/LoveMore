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
	lovemore.stage:addChild(obj);
​	**SpriteRenderer**

	img = love.graphics.newImage("pic.png");

	sp = lovemore.newGameObject();
	com = lovemore.newSpriteRenderer(img);
	sp:addComponent(com);
	sp.transform:setPosition(100, 100);
	sp.parent = lovemore.stage;
	
	sp2 = lovemore.newSprite(img);
	sp2.transform:setPosition(50, 50);
	sp2.parent = sp;

​	**SpineAnimator**

	spine = lovemore.newGameObject();
	spine.transform:setPosition(x, y);
	spine.transform.scale = 0.25;
	spine:addComponent(lovemore.newSpineAnimator("spine/spineboy.json", "spine/spineboy.atlas"));
	SpineAnimator.castFrom(spine:getComponent("SpineAnimator")):setAnimation(0, "run", true);
	lovemore.stage:addChild(spine);

