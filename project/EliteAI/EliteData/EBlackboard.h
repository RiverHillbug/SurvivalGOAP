/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EBlackboard.h: Blackboard implementation
/*=============================================================================*/
#pragma once

//Includes
#include <unordered_map>
#include <string>

namespace Elite
{
	//-----------------------------------------------------------------
	// BLACKBOARD TYPES (BASE)
	//-----------------------------------------------------------------
	class IBlackBoardField
	{
	public:
		IBlackBoardField() = default;
		virtual ~IBlackBoardField() = default;
	};

	//BlackboardField does not take ownership of pointers whatsoever!
	template<typename T>
	class BlackboardField : public IBlackBoardField
	{
	public:
		explicit BlackboardField(const std::string& name, const T& data) : m_Name{ name }, m_Data{ data }
		{}

		inline const std::string& GetName() const { return m_Name; }
		inline const T& GetData() const { return m_Data; };
		inline void SetData(const T& data) { m_Data = data; }

	private:
		const std::string m_Name;
		T m_Data;
	};

	//-----------------------------------------------------------------
	// BLACKBOARD (BASE)
	//-----------------------------------------------------------------
	class Blackboard final
	{
	public:
		Blackboard() = default;
		~Blackboard()
		{
			for (auto& el : m_BlackboardData)
				delete el.second;
			m_BlackboardData.clear();
		}

		Blackboard(const Blackboard& other) = delete;
		Blackboard& operator=(const Blackboard& other) = delete;
		Blackboard(Blackboard&& other) = delete;
		Blackboard& operator=(Blackboard&& other) = delete;

		template<typename T>
		bool GetData(const std::string& name, T& data) const
		{
			const auto& it = m_BlackboardData.find(name);
			if (it != m_BlackboardData.end())
			{
				if (const BlackboardField<T>* p = dynamic_cast<BlackboardField<T>*>((*it).second))
				{
					data = p->GetData();
					return true;
				}
			}

			return false;
		}

		template<typename T>
		bool SetData(const std::string& name, const T& data)
		{
			auto it = m_BlackboardData.find(name);
			if (it == m_BlackboardData.end())
			{
				m_BlackboardData[name] = new BlackboardField<T>(name, data);
				return true;
			}
			else
			{
				if (BlackboardField<T>* p = dynamic_cast<BlackboardField<T>*>((*it).second))
				{
					p->SetData(data);
					return true;
				}
			}

			return false;
		}

		bool GetBoolData(const std::string& name) const;

		void RemoveData(const std::string& name);

	private:
		std::unordered_map<std::string, IBlackBoardField*> m_BlackboardData;
	};
}
