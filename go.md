# 学习记录(go/八股)

## DAY 1

### http概述
> 简介: http协议一个**应用层协议,而不是传输层协议**.相比于TCP/UDP,http是更上一层的协议,本质上还是一个传输在网络上的字节流(比特流),是一个**被限定的字符串数据**,在原本发送的基础上添加了一些字符串信息的比特流数据,而不是什么特别难懂的玩意.相当于我在用socket(假设是C/C++)在传输一个**简单的char[]**时候,变成了传递一个有**特殊格式的char[]**而已,这些额外的格式信息就是http协议.相应的,SMTP/FTP协议的信息也是在基础的比特流上做了额外的限制而已.
> 也因为他是应用层协议,所以它是经过TCP/IP协议栈传输,TCP协议自动保证了连接的稳定和顺序收发,http只管发出即可,


要点:
1. http协议是应用层协议,而不是传输层协议.
2. http协议是**被限定的字符串数据**,而不是什么特别难懂的玩意.
3. 其他的应用层协议也是如此,不用想太多.

### 网络资源以及其网络中的表示与传输
> 网络资源在网络上以URI(统一资源标识符,URL是这玩意的子集)被定位,资源存放在各种各样的服务器上,用URI访问具体服务器的具体资源.URI 有两种形式,分别称为 URL 和 URN.
> 统一资源定位符（URL）是资源标识符最常见的形式.URL 描述了一台特定服务器上某资源的特定位置.它们可以明确说明如何从一个精确,固定的位置获取资源.比如http://www.baidu.com/index.html就是一个简单的URL实例. 当然ftp://的也是也URL.
> URI 的第二种形式就是统一资源名（URN）.URN 是作为特定内容的唯一名称使用的,与目前的资源所在地无关.使用这些与位置无关的 URN,就可以将资源四处搬移.通过 URN,还可以用同一个名字通过多种网络访问协议来访问资源.但是**URN 仍然处于试验阶段，还未大范围使用**,直接使用URL即可, 但是要注意之间的区别,尽量不混用.
**Q&A: http如何知道主机在哪里?**
答:URL中的host(主机名)就是服务器的域名,比如http://www.baidu.com/index.html,其中的www.baidu.com就是host.实际上是通过URL间接实现寻址进行访问的
> 当然也可以这样:http://207.200.83.29:80/index.html,直接使用IP代替域名.指定协议的时候,:80这个指定端口可省略.http默认就是80端口.

### http的事务
> http的**事务由一个客户端的请求,以及一个服务器的响应组成**.客户端请求由请求行,请求头,请求体组成,服务器响应由响应行,响应头,响应体组成.这种通信的内容是http报文

常见的http方法:GET POST PUT DELETE HEAD...还有其他方法,但是这五种是最常用的.
- GET:从服务器向客户端发送命名资源
- PUT:将来自客户端的数据存储到一个命名的服务器资源中去
- DELETE:从服务器中删除命名资源
- POST:将客户端数据发送到一个服务器网关应用程序
- HEAD:仅发送命名资源响应中的 HTTP 首部

一个浏览器页面,可能包含多个http事务,一个页面可能包含不同的资源,位于不同的服务器上.

### http报文
> HTTP 报文。HTTP 请求和响应报文的格式很类似。
HTTP 报文包括以下三个部分。
- 起始行:报文的第一行就是起始行，在**请求报文中用来说明要做些什么**，在**响应报文中说明出现了什么情况**。
- 首部字段:起始行后面有零个或多个首部字段。每个首部字段都包含一个名字和一个值，为了便于解析，两者之间用冒号（:）来分隔。**首部以一个空行结束**。添加一个首部字段和添加新行一样简单。
- 主体:空行之后就是**可选的报文主体**了，其中包含了所有类型的数据。请求主体中包括了要发送给 Web 服务器的数据；响应主体中装载了要返回给客户端的数据。起始行和首部都是文本形式且都是结构化的，而主体则不同，**主体中可以包含任意的二进制数据（比如图片、视频、音轨、软件程序）。当然，主体中也可以包含文本**。

``` HTML
HTTP/1.1 200 OK					-- 状态
Content-Type: text/HTML 		-- 两行响应头
Content-Length: 1234		
								-- 空行
<!DOCTYPE html>					-- Body,响应体
<html>
  <body>Hello World!</body>
</html>
```

✅ 大多数情况下，HTTP 响应的 Body 就是被请求的资源（如 HTML、图片、音乐等）。
⚠️ 但需注意：
- 检查状态码（如 200 成功，404 未找到）。
- 确认 Content-Type（如 text/html、audio/mp3）。
- 某些请求（如 HEAD、204、304）可能没有 Body。

如果是下载文件，Body 就是文件的原始数据；如果是 API，Body 通常是 JSON/XML。理解这一点对前后端开发和调试非常重要！

### Web的结构组件
> 前面的东西属于Web浏览器和服务器的内容,现在是其他的一些重要组件:代理/缓存/网关/隧道/Agent代理
HTTP 协议中的核心组件（如代理、缓存、网关、隧道、Agent 代理）在网络通信中扮演不同角色，它们协同工作以提升性能、安全性和功能性。以下是它们的详细作用和区别：

---

#### **1. 代理（Proxy）**
##### **作用**：
- **中介转发**：作为客户端和服务器的中间层，代理接收客户端请求并转发给服务器（或反向代理接收服务器请求转发给客户端）。
- **功能扩展**：
  - **匿名性**：隐藏客户端真实 IP（如 VPN 或匿名代理）。
  - **访问控制**：过滤非法请求（如企业防火墙）。
  - **内容过滤**：拦截广告或恶意内容。
  - **负载均衡**：将请求分发到多个服务器（反向代理）。
> 对用户来说,程序就是代理,程序代表用户访问服务器.而Nginx就是一种典型的反向代理,将客户端请求分发到后端服务器
##### **类型**：
- **正向代理**：代表客户端向服务器发送请求（客户端需显式配置代理地址）。
- **反向代理**：代表服务器接收请求（客户端无感知，如 Nginx）。

##### **示例**：
```http
客户端 → 正向代理 → 互联网 → 服务器
客户端 ← 正向代理 ← 互联网 ← 服务器
```

---

#### **2. 缓存（Cache）**
##### **作用**：
- **存储副本**：保存服务器响应的副本（如 HTML、图片、API 结果），减少重复请求和带宽消耗。
- **性能优化**：
  - **客户端缓存**：浏览器缓存静态资源（通过 `Cache-Control` 头控制）。
  - **代理缓存**：CDN 或反向代理缓存热门内容（如 Cloudflare）。

##### **关键 HTTP 头部**：
- `Cache-Control`：定义缓存策略（如 `max-age=3600`）。
- `ETag` / `Last-Modified`：验证资源是否过期。

##### **示例**：
```http
请求：GET /logo.png
响应：200 OK (from cache)  # 直接返回本地缓存，无需访问服务器
```

---

#### **3. 网关（Gateway）**
##### **作用**：
- **协议转换**：连接不同协议或技术的系统（如 HTTP ↔ SOAP、HTTP ↔ gRPC）。
- **功能整合**：
  - **API 网关**：聚合多个微服务接口（如 Kong、Apigee）。
  - **支付网关**：将 HTTP 请求转换为银行内部协议。
> 协议之间的切换.相当于是适配器的作用
##### **与代理的区别**：
- 网关是 **协议转换器**，代理是 **协议透传器**。

##### **示例**：
```http
客户端 → HTTP网关 → 银行内部系统（非HTTP协议）
```

---

#### **4. 隧道（Tunnel）**
##### **作用**：
- **穿透防火墙**：在受限网络中建立加密通道，封装并传输其他协议的数据（如 HTTP 隧道传输 SSH 流量）。
- **典型场景**：
  - **SSH 隧道**：`ssh -D 8080 user@server` 创建 SOCKS 隧道。
  - **VPN**：通过 HTTP 伪装 VPN 流量绕过封锁。

##### **特点**：
- 隧道不解析 HTTP 内容，仅作为数据传输管道。

##### **示例**：
```http
客户端 → HTTP隧道（加密） → 代理服务器 → 目标服务器
```

---

#### **5. Agent 代理（User Agent）**
##### **作用**：
- **客户端代表**：向服务器标识客户端类型和能力（如浏览器、爬虫、移动应用）。
- **功能**：
  - **自动协商**：通过 `User-Agent` 头声明支持的格式（如 `Accept: text/html`）。
  - **行为适配**：服务器根据 Agent 返回不同内容（如移动版/桌面版页面）。

##### **常见 User-Agent**：
- 浏览器：`Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36...`
- 爬虫：`Googlebot/2.1 (+http://www.google.com/bot.html)`

##### **示例**：
```http
GET / HTTP/1.1
User-Agent: curl/7.68.0  # 告知服务器客户端是 cURL
```

---

#### **组件对比总结**
| **组件**  | **角色**          | **典型应用场景**         | **关键特点**             |
| --------- | ----------------- | ------------------------ | ------------------------ |
| 代理      | 请求转发/过滤     | 匿名访问、负载均衡       | 可修改请求/响应          |
| 缓存      | 存储响应副本      | CDN、浏览器缓存          | 减少服务器负载，加速访问 |
| 网关      | 协议转换/接口聚合 | API 网关、支付系统       | 跨协议通信               |
| 隧道      | 加密穿透网络      | SSH over HTTP、企业VPN   | 不解析内容，纯传输       |
| Agent代理 | 标识客户端        | 浏览器、爬虫、移动端适配 | 通过 HTTP 头部声明能力   |

---

#### **协同工作示例**
1. **用户访问网站**：
   - **浏览器（Agent）** 发送请求 → **CDN（缓存+反向代理）** → 若未命中缓存，请求到达 **API 网关** → 网关调用后端微服务。
2. **企业安全访问**：
   - 员工通过 **HTTP 隧道** 连接公司内网 → 请求经过 **正向代理** 过滤 → 访问内部 **网关** 转换协议。

理解这些组件有助于设计高性能、安全的网络架构！

## DAY2

### TCP链接建立全过程

**1. 三次握手 :**

- 客户端向服务器发起请求, 发送一个SYN报文(请求建立), 附带一个seq报文, 假设是x, 

- 服务器如果接受到, 就会回复一个ACK报文, seq = x + 1, (和客户端的SYN作为一对), 然后也发送一个SYN报文, 表示同意建立(并发送序列号y) ,

- 客户端在收到SYN报文也会回复一个ACK.链接建立.

> 三次握手的作用是**确保双方都能进行正常的收发**.

**2. 四次挥手:**(假设A是先结束的一端,B是后结束收发的一端)

- A端发送FIN表示本端数据发送完毕(seq = x),进入 *FIN_WAIT_1* 状态.

- B端接受FIN之后,发送ACK(seq = x + 1),同时进入 *CLOSE_WAIT* 状态.A端接受到ACK之后进入 *FIN_WAIT_2* 状态.

- 当B处理完自身的数据之后, 也会发送一个FIN报文(seq = v),并且自身进入 *TIME_WAIT* 状态, 此时 A 接受FIN之后,发送一个ACK 到B ,然后等待 2MSL(Maximum Segment Lifetime)时间长度之后就关闭链接.

> 四次挥手是因为传输是双工的.需要两端都关闭.另外,晚关闭的(如假设的B端),有可能需要额外的时间处理本端的数据,比如A可能结束发送,但是B还有没发送的,B还需要继续发送. 另外, A等待的这个2MSL时间就是socket编程中地址复用的原因, **因为就算大多数TIME_WAIT是发生在客户端, 也还是会有发生在服务器的, 但是服务器是无法接受这样的等待的.因此服务器需要地址复用.**而不是因为是后发FIN的没有进入TIME_WAIT,还需要复用,只是有时候,先发FIN的是服务器而已.

### 谁建立的TCP链接?

建立TCP链接的是客户端的connect函数 + 服务器的listen函数,以及内核的协议栈.

- connect: 主动发起TCP链接,发送SYN报文, 触发3次握手机制

- listen: 监听端口, 允许内核接受链接请求.

- 内核协议栈: 完成3次握手, 维护链接队列.

> accept函数只是从已经连接成的TCP链接中取一个出来进行处理而已.**也就是说,程序中就算不写accept,一样能成功建立链接,但是没法正常处理罢了**. 因此, 如果**客户端在服务器执行accept之前就开始发送数据, 本质上是可以的**, 这些数据会被**存在缓冲区**中, 但是如果accept迟迟不肯调用, 则缓冲区满,**会触发TCP的流量控制机制(TCP是安全的协议 ,保证不重复,不丢失,按顺序到达.不要忘了)**, 直到accept被调用,缓冲区中的数据被处理为止.(当然了,也有极端情况--> Keep-Alive 超时.)
> 此时客户端会被阻塞或者报错,取决于是阻塞模式还是非阻塞模式:
> 	是否阻塞/报错取决于客户端模式：
> 		阻塞模式 → send() 卡死。
> 		非阻塞模式 → send() 失败并返回 EWOULDBLOCK。

## DAY3 

### go的结构体内嵌/方法内嵌


### go接口作为传参的经典问题辨析

```go
const debug = true	//当debug = false时候,触发panic:out.Write([]byte("done!\n"))

func main() {
	var buf *bytes.Buffer
	if debug {
		buf = new(bytes.Buffer) // enable collection of output
	}
	f(buf) // NOTE: subtly incorrect!
	if debug {
		// ...use buf...
	}
}
// If out is non-nil, output will be written to it.
func f(out io.Writer) {
	// ...do something...
	if out != nil {
		out.Write([]byte("done!\n"))
	}
}

```

在这个简单的例子中,如注释所说,当debug = false,时候,`out.Write([]byte("done!\n"))` 这里会发生panic.原因有以下几点:

- 空接口调用任何方法都会触发panic

- go的**接口**变量都有两个属性,**动态类型/动态值**,**两者只要有一个是nil,则方法不能被调用**.否则panic.

但是这是个经典问题,**最简单的办法就是将传入的 buf变量(类型原本是 *bytes.Buffer),设置为 io.Writer 接口**.因为这样, **函数形参就不会被传入的实参进行动态类型赋值**,从而在原本程序的流程上,out == nil 就恒成立了(因为此时接口的动态类型没有被赋值导致变化,依旧是nil.go中,接口只声明不赋值,则两个值都视为nil,即便是var inter io.Writer也一样),但是当实参在上述例子中不是io.Writer 类型的接口时候, 形参就发生了一次拷贝, 动态类型不再为nil, 整个 out 变量就不再是nil了, 然后就因为调用了nil动态值, 发生panic.

> 要点: 
> 1. 接口只声明不赋值,则两个值都视为nil,此时是真正的nil接口.两者有一个不是nil, 则接口不是nil接口
> 
> 2. 形参接口类型与实参不同, 会发生动态类型的拷贝.

### go接口的无缝替换使用原则

对于go, 只要实现了某个接口的函数签名, 就是实现了这个接口, 但是一个类实例可能同时实现多个接口的全部方法, 此时, 如果想接口互相转换, 就必须满足一个原则: **假设 A 实现的方法 > B 实现的方法数量,那么, B接口的实例可以赋值给 A ,但是A不能赋值给B接口类型,因为根本上说,B没有实现A所有的方法,B的实例不属于A接口.**

> 多方法的接口 不能  赋值给 少方法的接口 (前提当然是包含关系而不是交集关系): 
> abc := a     ----> 可以
> a:= abc      ----> 不行
> 很好理解,因为想把某个东西当作另一个东西使用,就需要实现目标的所有方法,让自己变成目标的类.

**有待补充**


### go的方法内嵌/结构体内嵌

1. **结构体内嵌**

将一个结构体作为另一个结构体的成员就会这样. 新的结构体直接可以访问匿名结构体的成员,相当于匿名结构体的成员被升格了. 不过 ,赋值的时候,需要写明匿名结构体的字段,如下例:

```go
type A struct {
    FieldA int
}

type B struct {
    A       // 匿名嵌入结构体A
    FieldB string
}

b := B{A: A{FieldA: 42}, FieldB: "hello"}

fmt.Println(b.FieldA) // 直接访问A的字段，输出42
```

嵌套冲突:

- 如果外层结构体和内嵌结构体有同名字段，**外层字段会优先**。(不可喧宾夺主)

- 必须显式指定内嵌结构体名访问被覆盖的字段：

```go
type C struct {
    A
    FieldA string // 与外层同名字段覆盖
}

c := C{A: A{FieldA: 100}, FieldA: "override"}
fmt.Println(c.FieldA)         // 输出 "override"（外层优先）
fmt.Println(c.A.FieldA)       // 输出 100（显式访问内嵌字段）
```

2. **方法内嵌**

内嵌结构体的方法,外层结构体现在也可以使用:

```go
func (a A) MethodA() {
    fmt.Println("MethodA called")
}

b := B{}
b.MethodA() // 直接调用A的方法，输出 "MethodA called"
// 等价于 b.A.MethodA()
```

- 如果外层结构体定义了同名方法，会**覆盖内嵌结构体的方法**(不可喧宾夺主, 永远是外层的优先)

- 通过内嵌结构体，外层结构体可以隐式实现接口

> 以上特性, 间接的实现了go的OOP特性.实现了继承.

## DAY4

### go的GMP和gc机制

#### GMP机制

这是go 强大并发机制的核心之一, GMP分别代表三个重要概念:

- M: 操作系统线程

- P: 逻辑处理器

- G: 被创造的协程

> 这三个值, P是稀缺资源, M >= P 的情况大多数并发情况是成立的, 单线程时候, 这两个数量是基本一一对应的. 不会出现P > M 的情况, 因为此时表示逻辑处理器闲置, **而go的调度器是会自动创建M 去配对P 的**, 因此这种情况永远不会发生, 这是由go的机制决定的.
>
> 既,并发时: M > P , 单线程时: M = P, 没有P > M 情况.

正常程序执行的流程, 是多个OS线程, 在go的逻辑处理器协助下, 处理多个go协程, 完成并发编程. 

- 默认情况下, P 的数量 由 GOMAXPROCS 决定（默认 = CPU 核数）,然后创建**相等数量的M** 与之匹配.

- 新创建的G, 会**被加入到P的本地队列**（Local Run Queue, LRQ） 中, 等待M处理前方排队的G之后处理. 若当前P队列满, **则会尝试进行负载均衡操作**.

- M只负责执行LRQ中的G, 只有当前的LRQ执行结束了,会**去GRQ中偷取任务执行**, 如果队列空, 则会**去别的队列偷取任务执行**.

> 负载均衡: 0.5概率丢给GRQ, 0.5概率丢给其他没满的LRQ, 如果自己的LRQ没满, 则不会触发负载均衡.

这就是go的GMP机制.

#### gc机制

Go 的垃圾回收（Garbage Collection, GC）机制是一种自动内存管理技术，用于回收不再使用的内存，防止内存泄漏。Go 的 GC 采用 **并发标记-清除（Concurrent Mark-Sweep）算法**，并在此基础上优化以减少 **STW（Stop-The-World）** 停顿时间。下面从 **基本原理、工作流程、优化策略、GC 调优** 等方面详细解析。



1. 基本原理:

    Go 的 GC 属于 **追踪式垃圾回收（Tracing GC）**，核心思想是：

   - **标记（Mark）**：从根对象（如全局变量、栈变量）出发，遍历所有可达对象，标记为“存活”。
   - **清除（Sweep）**：遍历堆内存，回收未被标记的“垃圾”对象。

   Go 的 GC 是 **分代无感知（Non-generational）** 的（Go 1.19 之前），但 Go 1.19+ 引入了 **分代 GC 实验性支持**（`GOGC=generational`）。

2. go的GC流程,4个阶段:

   - ### **`GC 开始`（STW）**

   1. stop the world:暂停所有的goroutine, 准备gc.
   2. 扫描 **栈、全局变量、寄存器** 等根对象，启动并发标记。 

   - ### **`并发标记`（Concurrent Marking）**

   1. **后台 goroutine** 遍历堆，标记所有可达对象。
   2. **写屏障（Write Barrier）** 确保并发标记期间，新分配的对象能被正确追踪。

   - ### **`标记终止`（Mark Termination, STW）**

   1. 再次暂停所有 goroutine，完成最终标记。
   2. 检查是否有遗漏的对象，确保标记准确性。

   - ### **`并发清除`（Concurrent Sweeping）**

   1. 回收未被标记的内存，归还给堆管理。
   2. 新分配的对象直接进入新的内存空间，不影响回收。

---

## DAY5 

### go 的range关键字

作用: 迭代各种集合类型的数据, 遍历集合中的数据, 为go的循环迭代提供了一种简洁高效的语法. 他可以作用的类型及其返回值分别是:

1. 数组/切片: 返回**索引和值**

2. 字符串: 返回字符**索引和 Unicode 码点**(rune)

3. map: 返回**键和值**

4. channel: **只返回值**

> 对于channel, **需要额外说明, 因为这个类型比较特殊**, range遍历通道是一种特殊用法, 会一直接受通道的数据, 直到这个通道被关闭. **不是遍历通道缓冲区, 而是持续监听通道, 接收所有后续发送到通道的值**, 执行for... range somechan {...}时候, **会阻塞当前的goroutine, 直到chan中来了数据. 另外, 直到chan被显式关闭, range过程才会结束**, 这一点与其他的数据的range是很有区别的.
>
> **要点**: 1.监听chan, 接收数据		2. 阻塞当前goroutine	3. chan被关闭时才结束range过程.并且是发送方负责关闭.接受方只负责处理读到的数据



### go的channel



Go 语言中的 **Channel（通道）** 是用于在不同 Goroutine（协程）之间进行安全通信和同步的核心机制，是 Go 并发模型的核心组件之一。以下是关于 Channel 的详细介绍：

---

#### **1. Channel 的基本概念**
- **通信机制**：Channel 是 Goroutine 之间的通信管道，遵循 **“先入先出（FIFO）”** 原则。
- **类型安全**：Channel 有明确的类型，只能传递指定类型的数据（例如 `chan int` 只能传递整数）。
- **并发安全**：Channel 的**发送和接收操作是原子的，无需额外加锁**。

---

#### **2. Channel 的声明与创建**
##### **声明语法**
```go
var ch chan T      // 声明一个类型为 T 的 Channel（此时 ch 为 nil）
ch := make(chan T) // 创建并初始化一个无缓冲 Channel
ch := make(chan T, n) // 创建并初始化一个容量为 n 的有缓冲 Channel
```

##### **类型示例**
```go
ch1 := make(chan int)     // 无缓冲 Channel
ch2 := make(chan string, 5) // 缓冲大小为 5 的 Channel
```

---

#### **3. Channel 的操作**
##### **基本操作**
| 操作             | 语法            | 行为描述                                                     |
| ---------------- | --------------- | ------------------------------------------------------------ |
| **发送数据**     | `ch <- value`   | 将数据写入 Channel。如果 Channel 已满（缓冲满或无缓冲），发送方会阻塞。 |
| **接收数据**     | `value := <-ch` | 从 Channel 读取数据。如果 Channel 为空，接收方会阻塞。       |
| **关闭 Channel** | `close(ch)`     | 关闭 Channel，关闭后不能再发送数据，但可以继续接收剩余数据。 |

##### **示例代码**
```go
func main() {
    ch := make(chan int, 2) // 缓冲大小为 2

    // 发送数据
    ch <- 1
    ch <- 2

    // 接收数据
    fmt.Println(<-ch) // 输出 1
    fmt.Println(<-ch) // 输出 2
}
```

---

#### **4. 无缓冲 Channel 与有缓冲 Channel**
##### **无缓冲 Channel（Unbuffered Channel）**

- **特点**：容量为 0，发送和接收必须同步（即“发送者”和“接收者”必须同时就绪）。
- **用途**：用于 Goroutine 之间的同步。
```go
ch := make(chan int) // 无缓冲

go func() {
    data := <-ch     // 接收数据（会阻塞直到发送方发送）
    fmt.Println(data)
}()

ch <- 42            // 发送数据（会阻塞直到接收方就绪）
```

##### **有缓冲 Channel（Buffered Channel）**
- **特点**：容量 > 0，发送操作在缓冲区未满时不阻塞，接收操作在缓冲区非空时不阻塞。
- **用途**：解耦发送和接收的速度差异。
```go
ch := make(chan string, 3) // 缓冲大小为 3

ch <- "A" // 不阻塞
ch <- "B"
ch <- "C" // 缓冲区满后发送会阻塞

fmt.Println(<-ch) // 输出 A（释放一个缓冲区位置）
```

---

#### **5. Channel 的关闭与检测**
##### **关闭 Channel**
- 通过 `close(ch)` 关闭 Channel。
- 关闭后，接收操作会立即返回剩余数据，之后返回零值（可通过第二个返回值检测是否关闭）。

**检测 Channel 是否关闭**

```go
value, ok := <-ch
if !ok {
    fmt.Println("Channel 已关闭")
}
```

##### **使用 `range` 遍历 Channel**
```go
for value := range ch {
    fmt.Println(value) // 自动检测关闭，退出循环
}
```

---

**6. 使用 `select` 处理多个 Channel**

`select` 语句用于同时监听多个 Channel 的读写操作，类似 `switch` 语法：
```go
select {
case data := <-ch1:
    fmt.Println("收到 ch1 的数据:", data)
case ch2 <- 42:
    fmt.Println("向 ch2 发送数据")
case <-time.After(1 * time.Second):
    fmt.Println("超时")
default:
    fmt.Println("没有 Channel 就绪")
}
```

**`select` 的特性**

- 随机选择一个就绪的 Case 执行（避免饥饿）。
- 常与 `for` 循环结合实现持续监听。
- `default` 分支用于非阻塞操作。

---

**7. Channel 的典型使用场景**

1. **协程间通信**  
   
   ```go
   func worker(ch chan int) {
       for num := range ch {
           fmt.Println("处理数据:", num)
       }
   }
   
   func main() {
       ch := make(chan int)
       go worker(ch)
       ch <- 1
       ch <- 2
       close(ch)
   }
   ```
   
2. **同步 Goroutine**  
   
   ```go
   func main() {
       done := make(chan bool)
       go func() {
           fmt.Println("任务完成")
           done <- true
       }()
       <-done // 阻塞等待子协程完成
   }
   ```
   
3. **生产者-消费者模型**  
   
   ```go
   func producer(ch chan int) {
       for i := 0; i < 5; i++ {
           ch <- i
       }
       close(ch)
   }
   
   func consumer(ch chan int) {
       for num := range ch {
           fmt.Println("消费:", num)
       }
   }
   ```
   
4. **限制并发数（信号量模式）**  
   
   ```go
   func main() {
       sem := make(chan bool, 3) // 最多允许 3 个并发
       for i := 0; i < 10; i++ {
           sem <- true
           go func(id int) {
               defer func() { <-sem }()
               fmt.Println("任务", id)
           }(i)
       }
       // 等待所有任务完成
       for i := 0; i < cap(sem); i++ {
           sem <- true
       }
   }
   ```

---

**8. 注意事项**

1. **避免向已关闭的 Channel 发送数据**（会导致 panic）。
2. **关闭 Channel 的职责**：通常由发送方关闭 Channel，接收方不应关闭。
3. **死锁问题**：确保发送和接收操作能匹配（例如无缓冲 Channel 必须有对应的接收方）。
4. **内存泄漏**：未关闭的 Channel 可能导致 Goroutine 无法被回收。

---

**9. 总结**

| 特性                 | 说明                                                    |
| -------------------- | ------------------------------------------------------- |
| **通信代替共享内存** | 通过 Channel 安全传递数据，避免竞态条件。               |
| **同步与异步**       | 无缓冲 Channel 用于同步，有缓冲 Channel 用于异步解耦。  |
| **多路复用**         | `select` 可同时监听多个 Channel，实现超时和非阻塞操作。 |

Channel 是 Go 并发编程的核心工具，合理使用可以简化并发逻辑，避免传统锁机制的复杂性。

---



## DAY6

### Go的init函数

#### 基本概念

init 函数是 Go 语言中的一个特殊函数，**每个包可以包含多个 init 函数，它们会在程序启动时自动执行**。

#### 主要特点

1. **自动执行**：不需要显式调用，Go 运行时自动调用
2. **执行时机**：在 main 函数**之前执行**
3. **多个 init**：一个包可以有多个 init 函数
4. **执行顺序**：
   - 按照依赖关系顺序执行（被导入包的 init 先执行）
   - 同一包中的 init 按源文件名字母顺序执行
   - 同一文件中的 init 按**声明顺序执行**

#### 使用场景

1. **初始化变量**：当变量初始化需要复杂逻辑时
   ```go
   var config map[string]string
   
   func init() {
       config = loadConfigFromFile()
   }
   ```

2. **注册功能**：如数据库驱动注册
   ```go
   func init() {
       sql.Register("mydriver", &MyDriver{})
   }
   ```

3. **验证配置**：确保包级别变量有效
   ```go
   func init() {
       if config.APIKey == "" {
           panic("API key is required")
       }
   }
   ```

#### 使用示例

```go
package main

import (
    "fmt"
)

var globalVar string

// 第一个init函数
func init() {
    globalVar = "Initialized"
    fmt.Println("First init:", globalVar)
}

// 第二个init函数
func init() {
    fmt.Println("Second init")
}

func main() {
    fmt.Println("Main function")
}
```

输出顺序：
```
First init: Initialized
Second init
Main function
```

#### 注意事项

1. init 函数**不能有参数和返回值**
2. 不要过度依赖 init 函数的执行顺序（除了同一文件中的顺序）
3. init 函数中的错误处理有限，通常只能 panic
4. 测试文件中也可以有 init 函数，会在测试前执行

init 函数是 Go 语言初始化机制的重要组成部分，合理使用可以使代码更加清晰和模块化。

---

## DAY7

### go的并发编程的常用包sync



**sync.Mutex**:标准互斥量,最基本的同步原语,**实际意思和C++的std::mutex是一样,**不需要过多的赘述.

特点有:

1. 零值就是未锁定的可使用状态
2. 不能重复获得
3. 不能被复制

**sync.RWMutex**: 读写锁,与C++一样,也是读操作可以多个线程同时进行,但是写操作则和Mutex一样.不可重入

```go
var rwmu sync.RWMutex
var data map[string]string

func read(key string) string {
    rwmu.RLock()         // 读锁定
    defer rwmu.RUnlock() // 读解锁
    return data[key]
}

func write(key, value string) {
    rwmu.Lock()         // 写锁定
    defer rwmu.Unlock() // 写解锁
    data[key] = value
}
```

特点:

1. 适合读多写少的场景.
2. 读不互斥,**写互斥(与读锁/写锁都互斥)**

>锁只是**约定不做修改操作(互斥锁和读写锁都是这个道理)**,如果我在锁的临界区(读写锁/互斥锁)做修改操作,编译器也检测不出来,只是最后可能得到的结果事与愿违,**锁诞生的意义本来也就是为了避免这个情况发生.**

---

## DAY8

### go的并发编程包sync一些其他常用的包

### **sync.WaitGroup(等待组)**: 

任务计数器,是sync包提供的一个同步工具,用于等待goroutine的执行,确保在任务执行完毕之后再执行剩余代码.

**sync.WaitGroup的方法**:

- **Add(delta int)**：此方法增加或减少 WaitGroup 计数器的值。通常，在创建一组工作之前调用 Add 方法来设置计数器的值为你要等待的工作单元数量。也可以通过传递负数来减少计数。
- **Done()**：等同于调用 `wg.Add(-1)`，用于标记一项工作的完成。每个工作单元完成后应该调用一次 Done 方法来递减计数器。
- **Wait()**：阻塞当前 goroutine 直到 WaitGroup 计数器归零。通常在主 goroutine 中调用 Wait 方法，以等待所有其他工作 goroutine 完成。

> ```go
> package main
> 
> import (
>     "fmt"
>     "sync"
>     "time"
> )
> 
> func worker(id int, wg *sync.WaitGroup) {
>     defer wg.Done() // 在函数退出时通知WaitGroup任务已完成
>     fmt.Printf("Worker %d starting\n", id)
>     time.Sleep(time.Second) // 模拟工作
>     fmt.Printf("Worker %d done\n", id)
> }
> 
> func main() {
>     var wg sync.WaitGroup	//声明sync.WaitGroup变量.
> 
>     for i := 1; i <= 5; i++ {
>         wg.Add(1) // 增加计数器，表示将有一个新的goroutine开始工作
>         go worker(i, &wg)
>     }
> 
>     wg.Wait() // 等待直到计数器变为0，即所有goroutine都完成了它们的工作
>     fmt.Println("All workers done")
> }
> ```
>
> 在这个例子中，我们启动了5个 goroutine 来模拟工作，并使用 `sync.WaitGroup` 来确保主程序会等待所有工作 goroutine 完成后再打印出 "All workers done"。



### **sync.Once**: 

一次性执行. 确保某个函数只执行一次,常用于初始化.

> ```go
> var once sync.Once
> var config map[string]string
> 
> func loadConfig() {
>     once.Do(func() {
>         config = make(map[string]string)
>         // 初始化配置...
>     })
> }
> ```
>
> 特点: 即使上述的Do函数多次调用, 最后也只有一次被执行,-----> 可以用来实现单例.

根据例子以及这个变量的特性,可以知道,sync.Once的Do函数传入的**对象是一个函数.而不是值.**另外,sync的这个结构体只有一个对应的方法,就是Do方法,没有sync.WaitGroup这样的3个方法.

> Do函数接受的传参是一个无返回值的函数对象,但是可以使用闭包的操作完成捕获外部变量的作用.例如:
>
> ```go
> package main
> 
> import (
> 	"fmt"
> 	"sync"
> )
> 
> var a1 = 10
> 
> func main() {
> 	var b1 = 20
> 	var one sync.Once
> 
> 	one.Do(func() {
> 		a1 += 2
> 		b1 += 4
> 	})
> 
> 	fmt.Println("a1 = ", a1, "b1 = ", b1)
> }
> 
> 
> //以下是shell执行程序的结果.
> //once@DOXE-Desktop ~/桌/g/testgo> ./testgo 
> //a1 =  12 b1 =  24
> //once@DOXE-Desktop ~/桌/g/testgo> 
> 
> ```
>
> 可见,a1和b1都被修改.



### **额外知识:闭包**

闭包是一个**函数与该函数创建时**的词法环境(lexical environment)组合而成的实体.这个“词法环境”指:**函数内部能访问到的外部变量,参数集合**。这意味着，即使函数在其原始作用域之外被调用，它仍然能够访问并操作这些外部变量(相当于存了一份快照)。



**闭包的工作原理:**在Go语言中，闭包允许一个函数捕获并持有其定义时的局部变量，即使这个函数在其原始的作用域之外被调用，它仍能访问并修改这些变量。这主要通过以下几点实现

1. **捕获变量**：当定义一个匿名函数时，如果该函数引用了其外层作用域中的变量，那么这个匿名函数就会捕获这些变量，并将它们保存起来。换句话说，这些变量成为了这个匿名函数的一部分。
2. **延长变量生命周期**：通常情况下，局部变量在离开其作用域后会被销毁。但是，如果存在一个闭包引用了这些局部变量，那么这些变量的生命周期将会被延长，直到不再有任何闭包引用它们为止。
3. **共享状态**：由于闭包持有对外部变量的引用，多个闭包可以共享并修改这些变量的状态。这使得闭包非常适合用于需要保持某些状态的场景。

例子:

```go
package main

import "fmt"

func makeCounter() func() int {
    count := 0 // 局部变量
    return func() int {
        count++ // 闭包捕获并修改了count变量
        return count
    }
}

func main() {
    counter := makeCounter()
    fmt.Println(counter()) // 输出: 1
    fmt.Println(counter()) // 输出: 2
}

//shell的输出:
//once@DOXE-Desktop ~/桌/g/testgo> go build .
//once@DOXE-Desktop ~/桌/g/testgo> ./testgo 
//1
//2
```

可见,count生命周期被延长.每个函数对象保存一份



### sync.Cond (条件变量)

用于goroutine间的通知机制，**基于锁的条件等待**。使用起来和std::conditionvariale相似

```go
var mu sync.Mutex
var cond = sync.NewCond(&mu)
var ready bool

func worker() {
    time.Sleep(time.Second)
    mu.Lock()
    ready = true
    cond.Signal() // 通知一个等待的goroutine
    mu.Unlock()
}

func main() {
    go worker()
    
    mu.Lock()
    
    for !ready {	// 防止虚假唤醒
        cond.Wait() // 等待条件满足
    }
    mu.Unlock()	
    fmt.Println("ready")
}
```

**sync.Cond 有4 个方法:**

1. **`NewCond(l Locker) *Cond`**：构造函数，**创建一个新的 Cond 实例并返回**，需要传入一个实现了 Locker 接口的锁（如 `*sync.Mutex` 或 `*sync.RWMutex`），这个锁将用于保护条件变量。
2. **`(c *Cond) Wait()`**：使当前 goroutine 等待条件变量变为 true。该方法首先解锁 Cond 关联的锁，然后阻塞当前 goroutine，直到另一个 goroutine 调用了 Broadcast 或 Signal 方法唤醒它。被唤醒后，在返回之前会重新加锁。
3. **`(c *Cond) Signal()`**：如果存在等待的 goroutines，那么随机唤醒其中一个。注意，即使没有 goroutine 在等待，调用 Signal 也不会产生错误。
4. **`(c *Cond) Broadcast()`**：唤醒所有正在等待 Cond 的 goroutines。同样，如果没有 goroutine 在等待，调用 Broadcast 也不会产生错误。

> 与C++一样,**需要防止虚假唤醒**,但是C++的虚假唤醒比go的轻松一些,因为C++的std::conditionvariale,2号参数可以直接写一个判断是否虚假唤醒的判断条件. 上例中,如果ready是true ,那么main routine就可以执行下去,反之就算mu获取了锁,也会进入for循环中执行wait,然后释放锁,沉睡下去,直到被Signal/Brodcast唤醒.

---

## DAY9

### go的内存分配机制

go的内存分配机制是编译器**自己决定将变量放在堆区或者是栈区**,与程序员用不用`new`关键字没有关系.**决定变量分配在堆区或者栈区的机制是逃逸分析**.

> 因此 ,go实际上没有语法直接指定分配位置, **由程序员的写法影响编译器的判断**,仅此而已.
>
> 另外 ,go的哲学之一是: 少操心内存.



- **逃逸分析**:Go 编译器在编译阶段会分析变量的**作用域是否逃逸到函数外部**，决定分配位置:
  - **不逃逸** → **栈上分配**（高效，自动回收）。
  - **逃逸** → **堆上分配**（由 GC 管理）。

> #### **触发逃逸的常见场景**
>
> - **返回指针或引用**：
>
>   ```go
>   func foo() *int {
>       x := 42 // x 逃逸到堆，因为返回值被外部引用
>       return &x
>   }
>   ```
>
> - **被闭包捕获**：
>
>   ```go
>   func bar() func() int {
>       y := 100 // y 逃逸到堆，因被闭包引用
>       return func() int { return y }
>   }
>   ```
>
> - **变量大小未知或动态**（如大对象、切片扩容）：
>
>   ```go
>   func baz() {
>       big := make([]int, 1e6) // 可能逃逸到堆（栈空间不足）
>   }
>   ```
>
> - **传递到全局变量或通道**：
>
>   ```go
>   var global *int
>   func qux() {
>       z := 10 // z 逃逸到堆
>       global = &z
>   }
>   ```



### go的io复用

**select**:go的内置语句,专门为channel操作设计的语句,使用起来类似switch语句,但是**只能用于channel**语句,**当case语句准备完毕,则会随机选取一个case执行,如果没有,就会阻塞;**

```go
package main

import (
	"fmt"
	"time"
)

// 判断管道有没有存满
func write(ch chan string) {
	for {
		select {
		case ch <- "hello":
			fmt.Println("write hello")
		default:
			fmt.Println("channel full")
		}
		time.Sleep(time.Millisecond * 500)
	}
}

func main() {
	// 创建管道
	output1 := make(chan string, 10)

	// 子协程写数据
	go write(output1)

	// 取数据
	for s := range output1 {
		fmt.Println("res:", s)
		time.Sleep(time.Second)
	}
}
```

根据这个例子,go的select不仅可以执行等待数据, 也能执行主动发送数据, 因此, 上述语句的执行结果是不断的触发:

```go
//1
case ch <- "hello":
	fmt.Println("write hello")

//2
for s := range output1 {
	fmt.Println("res:", s)
	time.Sleep(time.Second)
}
```

因此实际是无限循环,只是因为发送的频率更高吗,所以最后会阻塞一会在生产一个数据.说明**select会主动执行写channel**操作.

> 但是select用来写channel,则需要额外的同步控制语句,如果本例中不写time.Sleep(time.Millisecond * 500), 则会瞬间发送很多次"write hello"字符串.
>
> 所以正常情况就用select监听接受语句就好.
>
> 即
>
> ```go
> // ...
> x := <- ch
> // ...
> ```
>
> 的形式的代码.







---

## 专辑内容1: 接口的灵活使用

go的语法本身不难,但是go的接口实在是过于灵活,以至于领悟起来极其坐牢.

- **tips**:遇到包的函数不清楚的可以查官方帮助文档:  [https://pkg.go.dev](https://pkg.go.dev/?spm=5176.28103460.0.0.39601db8T3wHQf)  这里可以查看在线go文档和本地的.



go的变量总是被明确定义的**值初始化**,也就是说,一个变量一旦被声明出来,**实际就是声明 + 定义了零值**,而这种操作对于go的接口来说,就是定义了一个空接口.(动态类型和动态值都是nil)



> 调用空接口接口值的任何方法都会panic. 另外,空接口是**动态类型,动态值都是nil**的接口(  这里的空接口概念需要区分一下interface{} )

看一个经典例子:

```go
package main

import (
	"bytes"
	"fmt"
	"io"
)

const debug = false

func main() {
	var buf *bytes.Buffer
	if debug {
		buf = new(bytes.Buffer) // enable collection of output
	}
	f(buf) // NOTE: subtly incorrect!
	if debug {
		// ...use buf...
	}
}

// If out is non-nil, output will be written to it.
func f(out io.Writer) {
	// ...do something...
	if out != nil {
		fmt.Println("f 被执行了")	//任何情况下都执行了这一行.已经得到验证
		out.Write([]byte("done!\n"))
	}
}

```

对于这个例子, 不论怎样,` fmt.Println("f 被执行了")`这一行都是执行了的,也就是说,即使buf接口,最开始是完全的nil接口,最后也变成了不是空接口的接口,但是debug是true/false时候,有区别:

- debug= true时候,buf是一个new的**bytes.Buffer实例,而不是nil指针.**因此,这时候,动态类型是*bytes.Buffer,动态值是new(bytes.Buffer)---->底层的那个new出来的内存.
- debug=false时候, buf只是声明的变量,此时是编译器自动赋值为nil.绝对的空接口.

> 但是,在传入 f(out io.Writer)函数时候, **形参out发生了赋值**,因此两段代码都能进入if判断,而不是只有debug=false时进去,这就说明了, 形参out发生了赋值, 将buf的动态类型,动态值赋值给了out ,即使buf是空接口, buf原本的类型,bytes.Buffer也被赋值给了out的,因为out不是声明的.因此,out在任何情况下都不是nil,直接调用out.Write([]byte("done!\n")),在debug=false时候,会发生panic,因为buf没有实例,相当于nil指针调用了方法.





---

## 专辑内容2:常见包的使用

1. ### fmt包:

   - 关于print系列和scan系列(包括sprint/fprint), 使用起来和C语言的printf/scanf(fprint/fscanf  sprintf/sscanf)一样的规矩,不多扯了. 

     > 显然go的语法足够一目了然, Print系列和Scan系列的Println和Scanln使用起来都是十分方便的.

   - `fmt.Errorf(format string, a ...interface{}) error`：创建一个新的错误，带有格式化的错误消息。这是创建自定义错误信息的一种便捷方法。

2. **bufio包**:

   bufio 包本身主要由结构体组成，并没有定义额外的接口（因为它**依赖于 io 包**中的接口如 io.Reader 和 io.Writer）

   1. bufio.Reader: 带缓冲的读取器，封装了 `io.Reader`，用于高效读取数据。常用方法:**加粗为常用的**
      - **Read**(p []byte) (n int, err error)：读取数据到字节切片 `p`。
      - ReadByte() (byte, error)：读取单个字节。
      - ReadBytes(delim byte) ([]byte, error)：读取直到遇到分隔符 `delim`（返回字节切片，包含分隔符）。
      - **ReadString**(delim byte) (string, error)：与 `ReadBytes` 类似，但返回字符串（包含分隔符）。
      - Peek(n int) ([]byte, error)：预览接下来的 `n` 字节（不移动读取位置）。
      - UnreadByte()：回退一个字节到缓冲区。
      - UnreadRune()：回退一个 Unicode 码点到缓冲区。
      - **NewReader**(rd io.Reader) *Reader: 传入一个io.Reader接口(依赖io包),返回一个带默认缓冲区（4096字节）的 `*bufio.Reader`.
      - NewReaderSize(rd io.Reader, size int) *Reader: 同上,只是**可以自定义缓冲区大小**.
      
      > 它在原始数据源和代码之间插入了一个内存缓冲层，**通过智能填充/消费这个缓冲区来优化读取效率**。可以说,bufio.Reader是io.Reader和用户态缓冲区(可能是切片,也可能是其他)的一个中间缓冲区. **bufio.Reader优化的目标是:减少从底层读取的次数**.
      >
      > 所以Writer也是类似的道理. 不过API略有不同. **Reader和Writer在设计上是完全对称的**.有少许各自特有的API.









---



---go如何调试代码?

