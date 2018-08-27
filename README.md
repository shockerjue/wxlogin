# WXLogin
## WXLogin 
    采用grpc作为登录后台服务器
    mysql存储用户信息；
    redis缓存登录状态以及登录信息，每一个缓存项缓存时间为120秒，在收到用户的心跳信息，并且验证通过以后需要更新缓存信息，以保证不会过期;

    MYSQL和REDIS采用池进行管理，通过配置文件中的mysql_pool和redis_pool分别配置预先创建连接的数量，当有需要请求MYSQL和REDIS的时候，通过从池中提取一个连接对象进行数据读写。去连接对象的策略采用当前时间戳与池的大小取余进行选择。

## Build
    构建项目需要先安装bazel项目依赖与构建管理工具。 

    第一次构建时电脑需要能访问外网，有些依赖库需要从网络上下载。
    构建项目直接在项目根目录下执行: 
    Abc@Abc-computer:~$ bazel build :all

    构建完成以后，会在bazel-bin目录下生产对应的依赖库以及服务程序，其中client是RPC的测试程序。

## Run
    执行服务以前，需要确保安装了MYSQL和REDIS，其中还必须安装MYSQL开发库。

    在在运行以前确保项目正常构建成功。将根目录下的配置文件config.ini复制到服务在的目录下，并修改其中需要修改的参数，然后执行./server即可允许后台登录服务。
    Abc@Abc-computer:~$ ./server


## RPC接口
### 注册
    ```
     /**
     * 用户请求注册回调,注册成功没有说明已经登录了，仅仅是执行注册动作
     * 
     * @param context
     * @param request
     * @param reply
     */
    Status OnSignup(ServerContext* context,const SignupRequest* request,
        SignupReply* reply) override；
    ```
    注册需求对注册请求进行签名。注册成功不直接进行登录。注册密码采用md5加密存储。

### 登录
    ```
    /**
     *  用户登录调用
     * 
     * @param context
     * @param request
     * @param reply
     */
    Status OnLogin(ServerContext* context,const LoginRequest* request,
        LoginReply* reply) override
    ```
    登录需将登录信息进行签名。登录成功以后将登录状态和信息放到缓存中。如果登录的时候已经处于登录状态。则更新缓存中的登录信息和状态。

### 心跳
    ```
    /**
     * 用户在线心跳保持以及实时消息响应
     * 
     * @param context
     * @param request
     * @param reply
     */
	Status OnHeart(ServerContext* context,const HeartRequest* request,
		HeartReply* reply) override；
    ``` 
    用户维护登录状态，同时也用于获取用户的动态消息。维护登录状态使用mac验证，使用在其他设备上登录。

## 测试
### 运行服务
    Abc@Abc-computer:~/wxlogin/bazel-bin$ ./server
    [2018-8-27 11:22:26][E]   onLoad : config.ini open fail!

    说明没有将配置文件拷贝到目录中，将config.ini拷贝到目录中即可。
    Abc@Abc-computer:~/wxlogin/bazel-bin$ ./server
    server_host : 0.0.0.0
    server_port : 50051
    mysql_host : localhost
    mysql_port : 3366
    mysql_db : wxlogin
    mysql_user : root
    mysql_pass : 576188
    mysql_pool : 1
    redis_host : 127.0.0.1
    redis_port : 6379
    redis_db : 0
    redis_pass : 
    redis_pool : 1
    [2018-8-27 11:24:20][I]   InitConnection : 
    {
        host: localhost, 
        port: 3366, 
        dbname:wxlogin, 
        user: root, 
        pass: 576188
    } Connect Success!
    server_host : 0.0.0.0
    server_port : 50051
    mysql_host : localhost
    mysql_port : 3366
    mysql_db : wxlogin
    mysql_user : root
    mysql_pass : 576188
    mysql_pool : 1
    redis_host : 127.0.0.1
    redis_port : 6379
    redis_db : 0
    redis_pass : 
    redis_pool : 1
    Server Listening on 0.0.0.0:50051

    注：输出上面说明服务允许成功。

### 启动测试服务
#### 注册
    通过修改配置文件config.ini中的test-user域来修改测试数据。

    执行的动作：
    ./client 1      # 注册
    ./client 2      # 登录
    ./client        # 心跳


### 错误码
    enum StatusCode {
        OK              = 200,      // 成功
        ACCOUNT_EMPTY   = 2001,     // 账号为空
        NAME_EMPTY      = 2002,     // 用户名为空
        PASS_EMPTY      = 2003,     // 密码为空
        INFO_ERROR      = 2004,     // 信息错误
        SIGN_ERROR      = 2005,     // 签名错误
        PASS_ERROR      = 2006,     // 密码错误
        ACCOUNT_EXIST   = 2007,     // 账号已经存在
        LOGIN_ERROR     = 2008,     // 用户名或密码错误
        ACCOUNT_NOTEXIST= 2009,     // 账号不存在
        SIGNUP_FAILED   = 2010,     // 注册失败
        USER_ALREADY_LG = 2011,     // 账号已经登录
        LOGIN_OTHER_MAC = 2012,     // 账号在其他设备上登录
        NOT_LOGIN       = 2013,     // 账号没有登录或登录已经失效
    };
