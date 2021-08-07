#include"stdafx.h"
#include<functional>
#include<vector>
#include<thread>

/// <summary>
/// イベントの優先度定数
/// Priority_Min =末尾に追加
/// Priority_Min =先頭に追加
/// </summary>
enum ButiEventPriority {
    Priority_Min=-1, Priority_Max = -2, 
};

template<typename T>
class EventListenerRegister:public ButiEngine::IObject {
public:
    EventListenerRegister(std::function<void(T)>arg_func) { func = arg_func;  p_key = malloc(1); }
    ~EventListenerRegister() {
        delete p_key;
    }
    std::function<void(T)>GetFunc() { return func; }
    
    long long int GetKey()const {
        if (!p_key) {
            return -1;
        }

        return *(long long int*) & p_key;
    }
private:
    std::function<void(T)>func;
    void* p_key;
};


template <class T>
class EventMessenger
{
public:
	EventMessenger() {};

    ~EventMessenger()
    {
        Clear();
    };
    inline void Clear() {
        vec_functions.clear();
        vec_functionAndPriority.clear();
        for (auto itr = vec_functionIndex.begin(), end = vec_functionIndex.end(); itr != end; itr++) {
            delete* itr;
        }
        vec_functionIndex.clear();
        map_funtionIndexPtr.clear();
        minPriority = 0; 
        maxPriority = 0;
    }
	inline void Execute(const T& event)
    {
        if (!sorted) {
            CreateSortedVec();
        }
        for (auto function : vec_functions)
        {
            function(event);
        }
    }
    inline void Regist(std::shared_ptr<EventListenerRegister<T>> arg_func,const int priority=ButiEventPriority::Priority_Min)
    {
        long long int key = arg_func->GetKey();
        if(key<0){
            throw ButiEngine::ButiException(L"実体のないコールバック関数を登録しようとしています");
        }
        if (map_funtionIndexPtr.count(key)) {
            return;
        }
        int currentEvPriority = priority == ButiEventPriority::Priority_Max? currentEvPriority = maxPriority + 1: priority == ButiEventPriority::Priority_Min? currentEvPriority = minPriority: priority;
        


        vec_functionIndex.push_back(new int(vec_functionAndPriority.size()));
        map_funtionIndexPtr.emplace(key, vec_functionIndex.back());

        vec_functionAndPriority.push_back({ arg_func->GetFunc(),currentEvPriority });


        //優先度が最小、最大の場合は末尾、先頭に追加するのみ
        if (currentEvPriority <=minPriority) {
            minPriority = currentEvPriority;

            if (sorted) {
                vec_functions.push_back(arg_func->GetFunc());
            }

            return;
        }
        else if (currentEvPriority >maxPriority) {
            maxPriority=currentEvPriority;
            if (sorted) {
                vec_functions.insert(vec_functions.begin(), arg_func->GetFunc());
            }
            return;
        }

        sorted = false;
    }

    inline void UnRegist(std::shared_ptr<EventListenerRegister<T>> arg_func)
    {
        auto key = arg_func->GetKey();
        if (!map_funtionIndexPtr.count(key)) {
            return;
        }


        int index = *map_funtionIndexPtr.at(key);

        vec_functionAndPriority.erase(vec_functionAndPriority.begin() + index);
        map_funtionIndexPtr.erase(key);
        delete vec_functionIndex.at(index);

        for (auto indexItr = vec_functionIndex.erase(vec_functionIndex.begin() + index), end = vec_functionIndex.end(); indexItr != end; indexItr++) {
            *(*indexItr) -= 1;
        }
        sorted = false;
    }

private:

    inline void CreateSortedVec() {

        vec_functions.clear();

        std::vector<std::pair< std::function<void(T)>, int>> copied_vec_functionAndPriority=vec_functionAndPriority;

        std::sort(copied_vec_functionAndPriority.begin(), copied_vec_functionAndPriority.end(),
            [](std::pair< std::function<void(T)>, int> x, std::pair< std::function<void(T)>, int> y)->bool {return x.second > y.second; });

        vec_functions.reserve(copied_vec_functionAndPriority.size());
        for (auto itr = copied_vec_functionAndPriority.begin(), end = copied_vec_functionAndPriority.end(); itr != end; itr++) {
            vec_functions.push_back(itr->first);
        }

        sorted = true;
    }

    std::vector<std::pair< std::function<void(T)>, int>> vec_functionAndPriority;
    std::vector<std::function<void(T )>> vec_functions;
    std::vector<int*> vec_functionIndex;
    std::map<long long int,int*> map_funtionIndexPtr;
    std::map<int, int*> map_priority;
    int maxPriority=0, minPriority=0;
    bool sorted=true;
};

//イベント監視スレッド
class CountPerSecond
{
public:
    CountPerSecond()
    {
        isEnd = false;
        p_messenger = new EventMessenger<int>();
        thread = std::thread([&]() ->void
            {
                auto count = 0;
                while (true)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (isEnd)
                    {
                        break;
                    }
                    count++;
                    p_messenger->Execute(count);
                }
            });
    };

    inline void Regist
    (std::shared_ptr<EventListenerRegister<int>> arg_func,const int priority)
    {
        p_messenger->Regist(arg_func,priority);

    }
    inline void UnRegist
    (std::shared_ptr<EventListenerRegister<int>> arg_func)
    {
        p_messenger->UnRegist(arg_func);

    }

    void Dispose()
    {
        isEnd = true;
        thread.join();
        delete this;
    }
private:
    EventMessenger<int>* p_messenger;
    std::thread thread;
    bool isEnd;
    ~CountPerSecond()
    {
        delete p_messenger;
    };
};

class CountUp {
public:
    CountUp(const std::string& arg_name) {
        name = arg_name;
        rf = ButiEngine::ObjectFactory::Create< EventListenerRegister<int>>([&](int value) ->void {std::cout << name << value << std::endl; });
    }
    void Regist(CountPerSecond* arg_p_sub,const int priority) {

        arg_p_sub->Regist(rf,priority);
    }
    void UnRegist(CountPerSecond* arg_p_sub) {

        arg_p_sub->UnRegist(rf);
    }
private:
    int i = 0;
    std::shared_ptr<EventListenerRegister<int>> rf;
    std::string name;
};



int main() {
    

    auto count = new CountPerSecond();
    std::string outputMessage=":";
    CountUp c("priority0:"), c2("priority5:"), c3("priority2:"), c4("priorityMax:"), c5("priorityMin:");
    c.Regist(count, 0);
    getchar();
    c2.Regist(count, 5);
    getchar();
    c3.Regist(count, 2);
    getchar();
    c5.Regist(count, ButiEventPriority::Priority_Min);
    getchar();
    c4.Regist(count, ButiEventPriority::Priority_Max);
    getchar();


    count->Dispose();

    getchar();
	return 0;
}



