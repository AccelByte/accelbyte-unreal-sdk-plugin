#pragma once

template <typename T, typename U>
struct FBaseModelJustice
{
public:
	static T* Deserialize(U* item)
	{
		T* newToken = NewObject<T>();
		if (newToken->FromJson(item->ToJson()))
		{
			return newToken;
		}
		return nullptr;
	}

	static T* Deserialize(U item)
	{
		T* newToken = NewObject<T>();
		if (newToken->FromJson(item.ToJson()))
		{
			return newToken;
		}
		return nullptr;
	}

	static T* Deserialize(FString json)
	{
		T* newToken = NewObject<T>();
		if (newToken->FromJson(json))
		{
			return newToken;
		}
		return nullptr;
	}
};

