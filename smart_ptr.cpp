#include <iostream>
template<typename T>

class smart_share_ptr
{
public:
    smart_share_ptr(T *ptr = nullptr) : m_ptr(ptr) , m_count(new int(1)){}  //构造函数默认参数，常用来写在声明中，这个地方在定义里写也没有问题,如果构造函数不提供指定形参，则触发*ptr = nullptr的默认形参。
    ~smart_share_ptr() {
        if (--*m_count == 0) {
            delete m_ptr;
            delete m_count;
        }
    }
    
    T& operator*() const {  //const代表不改变成员变量
        return *m_ptr;
    }

    T* operator->() const {
        return m_ptr;
    }


    smart_share_ptr<T> (const smart_share_ptr<T>& rhs) noexcept {
    //smart_share_ptr<T> (const smart_share_ptr<T>& rhs) noexcept : m_ptr(new T), m_count(new int(1)) {     //noexcept 加在冒号与括号的中间 ,这里不能重新初始化成员函数再指向被拷贝对象，会造成内存泄漏

                                  //smart_share_ptr<type> smart_ptr2(smart_ptr);会直接触发拷贝构造
        this->m_count = &(++*rhs.m_count);
        this->m_ptr = rhs.m_ptr;
    }

    smart_share_ptr<T>& operator=(const smart_share_ptr<T>& rhs) noexcept {
        ++*rhs.m_count;                 //先++rhs 后判断是否为0是为了防止rhs = this的情况，这样在检查是否应该释放以前，计数器已经增加了，一加一减等于没变。
         if (--*this->m_count == 0) {
            delete m_ptr;
            delete m_count;
        }
        this->m_ptr = rhs.m_ptr;
        this->m_count = rhs.m_count;
        return *this;
    }

    //smart_share_ptr (const smart_share_ptr&) = delete;              //禁止掉拷贝构造，不禁止发生拷贝构造行为是默认走浅拷贝，导致析构时出错
    //smart_share_ptr& operator = (const smart_share_ptr&) = delete;   //禁止掉拷贝赋值

public:
    T *m_ptr;
    int *m_count;
};

class type{
   
public:
    type(int i = 0) : m_a(i) {}
    int m_a;
};

int main() 
{
    type *ptr_type = new type(1);
    smart_share_ptr<type> smart_ptr(ptr_type);
    std::cout<<smart_ptr->m_a<<std::endl;      // test operator ->
    std::cout<<(*smart_ptr).m_a<<std::endl;    // test operator *

    std::cout << "sptr's share_count：" << *smart_ptr.m_count << std::endl;

    smart_share_ptr<type> smart_ptr2(smart_ptr);
    std::cout << "sptr's share_count：" << *smart_ptr2.m_count << std::endl;

    smart_share_ptr<type> smart_ptr3;
    smart_ptr3 = smart_ptr2;
    std::cout << "sptr's share_count：" << *smart_ptr3.m_count << std::endl;
}