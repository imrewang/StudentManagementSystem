#pragma once


class CSocketInit
{
public:
    //构造
    CSocketInit();

    //析构
    ~CSocketInit();

    //打开网络库
    bool Startup();

    //关闭网络库
    bool Cleanup();
};