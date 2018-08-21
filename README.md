# LoveMore
Native extensions for LÖVE

​	**SpineAnimator**

	x = love.graphics.getWidth() / 2;
	y = love.graphics.getHeight() / 2;
	
	spine = lovemore.newSpineAnimator("spine/spineboy.json", "spine/spineboy.atlas");
	spine:setAnimation(0, "run", true);

	function love.update(dt)
	    spine:update(dt);
	end

	function love.draw()
	    love.graphics.draw(spine:getDrawable(), x, y, 0, 0.25, 0.25, 0, 0);
	end