#ifndef SET_RESTORE_GUARD_H
#define SET_RESTORE_GUARD_H

#include <assert.h>

template<class T> 
struct set_restore_guard
{
	set_restore_guard() : is_stores(false) {};
	
	set_restore_guard(T& var, T const new_val) :
		is_stores(true),
		var_ptr(&var) 
	{
		new (old_val) T(var);
		var = new_val;
	};
	
	~set_restore_guard() 
	{	
		*var_ptr = old_value();
		var_ptr = nullptr;
		is_stores = false;
	};
	
	void set(T& var, T const& new_val) 
	{
		if (is_stores)
			restore();
		is_stores = true;
		new (old_val) T(var);
		var_ptr = &var;
		
		var = new_val;
	}
	void restore() 
	{
		assert(is_stores);
		*var_ptr = old_value();
		var_ptr = nullptr;
		is_stores = false;
	}

	T const& old_value() const 
	{
		assert(is_stores);
		char ret_val[sizeof(T)];
		for (size_t i = 0; i < sizeof(T); i++)
			ret_val[i] = old_val[i];
		return  reinterpret_cast<T &>(ret_val);
	}
	explicit operator bool() const 
	{
		return is_stores;
	}

private:
	char old_val[sizeof(T)];
	T * var_ptr;
	bool is_stores;
};

#endif // !SET_RESTORE_GUARD_H
