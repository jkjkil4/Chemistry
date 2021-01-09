# Chemistry  
 化学配平工具  
 
## 使用方法  

#### 在"反应物与生成物"输入框中标明反应物和生成物
- 需要类似于 "**化学式\{电荷总数}**" \(不带引号) 的格式\(电荷总数不能省略数字，可以省略正号，正负号写在数字前面)  
如果电荷总数为0，可以简写为 "**化学式**" \(不带引号) 的格式
    - 例子:
        - **H2SO4** 可以写成 **H2SO4\{0}** 也可以写成 **H2SO4**
        - **Fe 2+** (带2正电荷) 需写成 **Fe\{+2}**
        - **MnO4 -** (带1负电荷) 需写成 **MnO4\{-1}** (不能写成 **MnO4\{-}**)
		- **K3\[Fe\(CN)6]** 需写成 **K3\(Fe\(CN)6)\{0}** (原化学式中各种括号都要改成小括号)
		- **CuSO4·H2O** 可以写成 **CuSO4_H2O\{0}** 也可以写成 **CuSO4_H2O** (原化学式中的 "**·**" 要改为 "**_**")
- 对于反应物，不需要额外加任何符号
- 对于生成物，需要在化学式的开头加上符号 "**$**" (不带引号)
	
#### 点击"配平"按钮即可配平
  
## 例子:
- 例子1
    - "反应物与生成物"输入框:
        - C  
        O2  
        $CO
    - 配平结果:
        - 反应物: 2 C + 1 O2
        - 生成物: 2 CO
- 例子2
    - "反应物与生成物"输入框
        - Cu  
        H\{+1}  
        NO3\{-1}  
        $Cu\{+2}  
        $NO  
        $H2O
    - 配平结果
        - 反应物: 3 Cu + 8 H{+} + 2 NO3\{-}
		- 生成物: 3 Cu\{2+} + 2 NO + 4 H2O
- 例子3
	- "反应物与生成物"输入框
		- Mn\[+7:a]O4\{-1}  
		H2O\[-1:b]2  
		H\{+1}  
		$Mn\[+2:a]\{+2}  
		$O\[0:b]2  
		$H2O
	- 配平结果
		- 反应物: 2 MnO4\{-} + 5 H2O2 + 6 H\{+}
		- 生成物: 2 Mn\{2+} + 5 O2 + 8 H2O
  
## 其他
- 化学式完全可以随便写，只要符合元素的第一个字母大写，其余小写即可
- 支持原子守恒、电荷守恒和得失电子守恒(得失电子守恒需要进行额外标注)
- 不支持带字母 如 \(...)n
- 不含物质状态和反应条件等信息
- ~~我觉得图标有点丑()~~
- 该项目使用 Qt 开发  
- 该项目使用了 [MyQtLibs](https://github.com/jkjkil4/MyQtLibs)

