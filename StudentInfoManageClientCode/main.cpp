#include "StartWork.h"

int main()
{
    CStartWork StartWork;

    // 启动数据库
    if (!StartWork.StartDatabase())
    {
        system("pause");
        return 0;
    }

    // 启动网络库
    if (!StartWork.StartSocket())
    {
        system("pause");
        return 0;
    }

    return 0;
}