# Chemistry  
 化学配平工具  
  
## 使用方法  
 1. 在左侧"化学式"输入框中定义化学式  
    - 需要在每个元素的后面使用 "\[化合价]" \(不带引号) 的格式标注其化合价\(不能省略数字)
    - 例子:
        - 硫酸 **H2SO4**，应写成 **H\[+1]2S\[+6]O\[-2]4**
        - 氢氧化钙 **Ca\(OH)2**，应写成 **Ca\[+2]\(O\[-2]H\[+1])2**
        - 亚铁离子 **Fe 2+** (总共带2正电荷)，应写成 **Fe\[+2]**
        - 高锰酸根 **MnO4 -** (总共带1负电荷)，应写成 **Mn\[+7]O\[-2]4**
        - 五水合硫酸铜 **CuSO4·H2O**，应写成 **Cu\[+2]S\[+6]O\[-2]4\_H\[+1]2O\[-2]**
        - 铁氰化钾 **K3\[Fe\(CN)6]**，应写成 **K\[+1]3\(Fe\[+3]\(C\[+2]N\[-3])6)**
 2. 在右侧"反应物与生成物"输入框中标明反应物和生成物
    - 需要用到前一步定义的化学式
    - 不需要像第一步一样给所有元素标明化合价，只需要在整个式子的末尾用 "\{电荷总数}" \(不带引号) 的格式标注其总电荷数\(不能省略数字)
        - 例子:
            - **H\[+1]2S\[+6]O\[-2]4** 只需写成 **H2SO4\{0}** 即可
            - **Fe\[+2]** 只需写成 **Fe\{+2}** 即可
            - **Mn\[+7]O\[-2]4** 只需写成 **MnO4\{-1}** 即可 (不能写成 **MnO4\{-}**)
    - 对于反应物，不需要额外加任何符号
    - 对于生成物，需要在化学式的开头加上符号 "$" (不带引号)
 3. 点击"配平"按钮即可配平(目前仅支持原子守恒和电荷守恒)
 4. 例子:
    - 例子1
        - "化学式"输入框:
            - C\[0]  
            O\[0]2  
            C\[+2]\[-2]
        - "反应物与生成物"输入框:
            - C\{0}  
            O2\{0}  
            $CO\{0}
        - 配平结果:
            - 反应物: 2 C + 1 O2
            - 生成物: 2 CO
    - 例子2
        - "化学式"输入框
            - Cu\[0]  
            H\[+1]  
            N\[+5]O\[-2]3  
              
            Cu\[+2]  
            N\[+2]O\[-2]  
            H\[+1]2O\[-2]
        - "反应物与生成物"输入框
            - Cu{0}  
            H{+1}  
            NO3{-1}  
              
            $Cu{+2}  
            $NO{0}  
            $H2O{0}
        - 配平结果
            - 略
