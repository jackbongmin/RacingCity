#pragma once

template<typename T>
class Singleton
{
public:
	static T& Get()
	{
		static T instance;
		return instance;
	}

protected:

	Singleton() = default;
	virtual ~Singleton() = default;

	Singleton(const Singleton&) = delete;				// 복사 생성자 삭제
	Singleton& operator=(const Singleton&) = delete;	// 대입 연산자 삭제
	Singleton(const Singleton&&) = delete;				// 이동 생성자 삭제
	Singleton& operator=(const Singleton&&) = delete;	// 이동 대입 연산자 삭제

};