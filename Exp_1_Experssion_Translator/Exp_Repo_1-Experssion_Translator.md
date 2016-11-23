## 编译原理课程实验报告

> 实验1 - 表达式翻译器
>
> 实验时间: 9th Oct, 2016
>



##### 1) 实验内容与目的

实验1.3要求使用语法制导定义方法，构造一个从中缀表达式到后缀表达式的带词法分析的翻译器，藉此初步实现一个简单的词法分析器与语法制导定义的程序框架，了解递归下降语法分析原理与语法制导定义的翻译过程。

##### 2) 实现分析

实验输入为包含有 +, -, *, /, 配对的括号、常数及以字母开头且只包含字母与数字的变量名的合法中缀表达式。

实验要求输出对应的不含括号的后缀表达式。

考虑到存在 (), */, +- 三种运算优先级，对原始文法消除左递归后得到语法制导翻译方案：

```Grammar
        expr    --> term rest
        rest    --> + term {print'+'} rest
                 |  - term {print'-'} rest
                 |  EMPTY
        term    --> factor section
        section --> * factor {print'*'} section
                 |  / factor {print'/'} section
                 |  EMPTY
        factor  --> ( expr )
                 |  id              {print(lexeme)}
                 |  num             {print(tokenVal)}
```
程序主体由词法分析器、match() 函数与语法制导翻译部分组成。对于一个字符输入流，一般的翻译流程如下：

1. GetToken() 函数（词法分析部分）将从输入流获取一个词素。对于空字符，函数将继续读入下一个词素；对于常数，tokenVal的值将被设置为常数值，函数返回 _TKN_NUM；对于变量名，字符串 lexeme 的值将被设置为变量名，函数返回 _TKN_ID；其他情况下函数将返回当前字符的 ASCII 值（合法情况下是 +-*/() 中的某一字符）。
2. 主程序调用语法制导翻译函数 Expr()，Expr() 函数调用其他语法制导翻译函数，直到完成翻译或出错终止。语法制导翻译函数要么根据 lookAhead 的值进行对应翻译动作（相当于终结符），要么调用其他语法翻译制导函数（相当于非终结符）。翻译过程中，完成对一个词素的匹配后函数将调用 match() 函数来将 lookAhead 更新为下一词素；match() 中期望的值与当前 lookAhead 不符时，说明输入表达式有误。

##### 3) 实验结果

a. 正常翻译

​	Input in-fix expression:15 + ((31 * t1) - 5) / (6 - 1)
​	Translated post-fix expression:15 31 t1 * 5 - 6 1 - / + 
​	Translated successfully.

b. 表达式未正常结束（ lookAhead 期望遇到结束标记 '\n' 但是遇到了未处理的字符）

​	Input in-fix expression:1 + 2 * 9 g 
​	Translated post-fix expression:1 2 9 * + Illegal input.(lookAhead: X)

c. Factor 项错误（ lookAhead 期望遇到 Factor 但是遇到了非 Factor 词素）

​	Input in-fix expression:1 + 3 * ( 2 + 6 ) + *
​	Translated post-fix expression:1 3 2 6 + * + Factor Error.(LookAhead: *)

d. Match 失配 （对于固定模式 match() 没有匹配到期望的值，如括号不匹配等）

​	Input in-fix expression:( 1 * ( 2 -9 ) + 5 / ( j1 - p2) ( 

​	Translated post-fix expression:1 2 9 - * 5 j1 p2 - / + Match failed.(LookAhead: ()

##### 4) 注意事项

1. 文法的左递归问题：对于存在左递归的文法，因为计算机内存中堆栈大小有限，过长的表达式有可能造成堆栈溢出的问题。为避免这种情况可以对原始文法进行左递归消除。
2. getToken() 超前读入的问题：getToken() 在识别词素时每次读入一个字符的方法，对于多字符变量则结合 now 变量的标记来处理。这样会造成 getToken() 在读入一个与 now 变量不同类的字符后才会结束当前词素匹配的问题，换句话说 getToken() 多从输入流读入了一个不属于当前词素的字符来确定已经完成当前词素的匹配。一个可行的解决方案是完成当前字符匹配并读入下一字符后，使用 ungetc() 函数将多读入的字符送回输入流。

