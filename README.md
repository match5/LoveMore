# LoveMore
Native extensions for LÖVE

​	**GameObject and Component**

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
	spine.transform:setPosition(x + 200, y + 50);
	spine.transform.scale = 0.5;
	spine:addComponent(lovemore.newSpineAnimator("spineboy.json", "spineboy.atlas"));
	lcoal comSpine = lovemore.newComponent({
		start = function(self)
			self.spAnim = SpineAnimator.castFrom(self.owner:getComponent("SpineAnimator"));
			self.spAnim:setAnimation(0, "walk", false);
			self.spAnim:setAnimationListener("complete", function(track, loopCount)
				if trackIndex == 0 then
	        self.spAnim:setAnimation(1, "jump", false);
	      elseif trackIndex == 1 then
	        self.spAnim:setAnimation(0, "walk", false);
	      end
	    end);
	  end
	});
	spine:addComponent(comSpine);
	lovemore.stage:addChild(spine);

