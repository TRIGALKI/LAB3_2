#ifndef HASHCOUNTER_H
#define HASHCOUNTER_H

// Класс "Хэш-таблица", основанная на методе цепочек
// Метод цепочек заключается в следующем: таблица представляет собой массив
// связных списков фиксированного размера. Вычисленный хэш-функцией хэш является
// индексом в этом массиве списков. Известно, что список по этому индексу будет
// содержать все идентификаторы, для которых функция вернула одинаовый хэш.
// Осталось только найти идентификатор в данном списке и возвратить ссылку на
// него.
#include "Identifier.h"
#include "vector"
#include "string"
#include "fstream"
template <class T>
class hashCounter
{
private:
    unsigned static const defaultCapacity = 16;
public:
    unsigned capacity;//размер хэш таблицы
    std::vector<std::list<Identifier<T>>>m_hash_table;
    size_t hashIntKey(const Identifier<T> &id) const
    {
        unsigned a = hash(id);
        return size_t(a%capacity);
    }
    hashCounter()
    {
    capacity = defaultCapacity;
    m_hash_table.resize(defaultCapacity,std::list<Identifier<T>>());
    for (int i = 0;i<int(this->m_hash_table.capacity());i++)
    {
        m_hash_table[i].clear();
    }
    }
    hashCounter(unsigned divisor)
    {
        capacity = divisor;
        m_hash_table.resize(capacity,std::list<Identifier<T>>());
         for (int i = 0;i<int(this->m_hash_table.capacity());i++)
         {
             m_hash_table[i].clear();
         }
    }
    ~hashCounter()
    {
        m_hash_table.clear();
    }

    hashCounter(hashCounter &cop)
    {
        this.hashCounter(cop.capacity);
        m_hash_table = cop.m_hash_table;
    }

    // Добавление идентификатора в хэш-таблицу
    void add(const T &id)
    {
        Identifier<T> a(id);
        size_t pos = hashIntKey(a);
        std::list<Identifier<T>>& line = m_hash_table[pos];
        typename std::list<Identifier<T>>::iterator it ;
        if (line.empty())
        {
            line.push_back(a);
        }
        else
        {
        bool flag = false;
        it = line.begin();
        while (it != line.end()&&flag == false)
        {
            if ((*it).getName() == id)
            {

                int counter = (*it).getCounter();
                counter++;
                (*it).changeCounter(counter);
                flag = true;
            }
            it++;
        }
        if (flag == false)
        {
            line.push_back(a);
        }
        }
    }
// Поиск идентификатора в таблице по имени
    Identifier<T> &get(const T &id_name)
    {
        // Сохраняется ссылка на список, в котором потенциально будет
        // расположен идентификатор (для простоты)
        Identifier<T> id(id_name);
        std::list<Identifier<T>>& line = m_hash_table[hashIntKey(id)];
        // Поиск идентификаторы в списке по имени
        typename std::list<Identifier<T>>::iterator it ;
        it = line.begin();
        while (it != line.end())
        {
            if ((*it).getName() == id.getName())
                return *it;
            it++;
        }
        // Если при поиске были просмотренны все элементы списка,и ни один не
        // подошёл - возвращаем указатель на пустой идентификатор
        if (it == line.end()){
            auto var = new Identifier<T>();
            return *var;
        }

        // Если идентификатор найден - возвращаем ссылку на него
        return *it;

   }
   void deletKey(const T &id_name)
   {
       Identifier<T> id(id_name);
       std::list<Identifier<T>>& line = m_hash_table[hashIntKey(id)];
       // Поиск идентификаторы в списке по имени
       typename std::list<Identifier<T>>::iterator ti;

       for (ti = line.begin();ti != line.end();)
       {
           if ((*ti) == id)
           {
                  ti = line.erase(ti);
           }
           else
           {
                ++ti;
           }
       }
   }
   int numObjects()
   {
       int sum = 0;
       typename std::list<Identifier<T>>::iterator it;
       for (unsigned i = 0;i<m_hash_table.capacity();i++)
       {
           std::list<Identifier<T>>& line = m_hash_table[i];
           for(it = line.begin();it != line.end();it++)
           {
               sum = sum+(*it).getCounter();
           }
       }
       return sum;
   }
   void clearCounter()
   {
       for (int i = 0;i<int(this->m_hash_table.capacity());i++)
       {
           m_hash_table[i].clear();
       }
   }
   bool isInCounter(const T&id_name) const
   {
        unsigned pos = hashIntKey(id_name);
        if (this->m_hash_table[pos].empty())
        {
            return false;
        }
        else
        {
            for(auto it = this->m_hash_table[pos].begin();
                it != this->m_hash_table[pos].end();it++)
            {
                if((*it).getName() == id_name)
                    return true;
            }
            return false;
        }

   }
   bool operator == (const hashCounter<T> &right) const
   {
       if (m_hash_table == right.m_hash_table)
           return true;
       else
           return false;
   }
   int operator [](const T&id_name)
   {
       Identifier<T> manup = this->get(id_name);
       return manup.getCounter();
   }


   hashCounter operator || (hashCounter<T> &right)
    {
       hashCounter<T> newhs;
        for (int i = 0;i<this->capacity;i++)
        {
            std::list<Identifier<T>>& line = m_hash_table[i];
            typename std::list<Identifier<T>>::iterator it;
                for(it = this->m_hash_table[i].begin();
                    it != this->m_hash_table[i].end();it++)
                {
                    if(!(line.empty()))
                    {
                    T path =(*it).getName();
                    if (right.isInCounter(path))
                    {
                        int a = (*it).getCounter();
                        Identifier<T> b = right.get((*it).getName());
                        int a1 = b.getCounter();
                        int sum = a+a1;
                        newhs.add((*it).getName());
                        newhs.get((*it).getName()).changeCounter(sum);
                    }
                    else
                    {
                        newhs.add((*it).getName());
                        newhs.get((*it).getName()).changeCounter((*it).getCounter());
                    }
                    }
                }
                for(it = right.m_hash_table[i].begin();
                    it != right.m_hash_table[i].end();it++)
                {
                    if(!(right.m_hash_table[i].empty()))
                    {
                    T path =(*it).getName();
                    if ((this->isInCounter(path)) == false)
                    {
                        newhs.add((*it).getName());
                        newhs.get((*it).getName()).changeCounter((*it).getCounter());
                    }
                    }
                }

            }

          return newhs;
        }
   void operator >>(std::string fileName)
   {
        std::ofstream fout(fileName);
        for (int i = 0;i<int(this->m_hash_table.capacity());i++)
        {
         if (!(this->m_hash_table[i].empty()))
         {
            typename std::list<Identifier<T>>::iterator it;
            for(auto it = this->m_hash_table[i].begin();
                it != this->m_hash_table[i].end();it++)
            {
                fout << (*it).getName() << std::endl;
                fout << (*it).getCounter() << std::endl;
            }
         }
        }
        fout.close();
   }
   void operator <<(std::string fileName)
   {
       std::ifstream fin(fileName);
       char keyBuf[81];
       char valueBuf[81];
       this->clearCounter();
       while(!fin.eof())
       {
           fin.getline(keyBuf,81);
           fin.getline(valueBuf,81);
           this->add(std::string(keyBuf));
           (this->get(std::string(keyBuf))).changeCounter(atoi(valueBuf));
       }
       fin.close();
   }
   void printData() const

   {    for (int i = 0;i<int(this->m_hash_table.capacity());i++)
       {
           if (!(this->m_hash_table[i].empty()))
           {
              typename std::list<Identifier<T>>::iterator it;
              for(auto it = this->m_hash_table[i].begin();
                  it != this->m_hash_table[i].end();it++)
              {
                  std::cout<<"Name :"<< (*it).getName() << std::endl;
                  std::cout <<"Count :"<< (*it).getCounter() << std::endl;
                  std::cout << "______________________ " << std::endl;
              }
            }
        }

   }

};
#endif // HASHCOUNTER_H

