
#ifndef scope_guard_h
#define scope_guard_h

#include <functional>


class scope_guard {
	
public:
	
	typedef std::function<void(void)> FunType;
	
	scope_guard(FunType fun)
	:_fun(fun) {}
	
	~scope_guard()
	{
		_fun();
	}
	
public:
	
	FunType _fun;
};

#endif /* scope_guard_h */
