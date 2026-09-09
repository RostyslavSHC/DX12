// Don't include this file more than once (Read it only once)
// pragma once - Is the same as #ifndef #define #endif
#pragma once

// T - Whatever class we want to make a Singleton
// T is just a temporary name for that class
// So this one Singleton code can work with many different classes.

// Create a reusable Singleton blueprint that can work with whatever class T is
template<typename T> // Make the class below a template (so that we can use it with any class)
class Singleton // Singleton class
{
public: // Everything is public inside this base class
	static T& GetInstance() // This creates a function called: GetInstance() which returns a reference to an object of type T
		// Normally you might need: object.DoSomething();       But because this is static, you can call: WindowController::GetInstance();
	{
		static T instance; // Static variable of type T (T is whatever class we want to make a Singleton)
		return instance; // Return the instance
		// static - This means that the variable will only be created once, and it will be shared across all instances of the class
	}

protected: // Everything is protected inside this base class (so that only derived classes can access it)
	Singleton() {} // Default constructor (Runs when the Singleton is created for the first time)
	virtual ~Singleton() {} // Virtual destructor (destruct when the Singleton is destroyed)

public: // Everything is public inside this base class
	Singleton(Singleton const&) = delete; // Prohibit you from copying the Singleton (so that you can't do: Singleton s2 = s1)
	Singleton& operator=(Singleton const&) = delete; // Prohibit assign/copy one Singleton into another Singleton (so that you can't do: s2 = s1)

};