
namespace engine
{
	template<typename T>
	void MemoryManager<T>::AddObject(T* Object)
	{
		m_Objects.emplace_back(Object);
	}

	template<typename T>
	bool MemoryManager<T>::RemoveObject(T* Object)
	{
		typename std::vector<T*>::iterator itr = std::find(m_Objects.begin(), m_Objects.end(), Object);
		if (itr != m_Objects.end())
		{
			delete (*itr);
			m_Objects.erase(itr);
			return true;
		}
		else
			return false;
	}

	template<typename T>
	MemoryManager<T>::~MemoryManager()
	{
		for (unsigned int i = 0; i < m_Objects.size(); i++)
			if(m_Objects[i] != nullptr)
				delete m_Objects[i];
	}
}