#穷举华容道   
代码写的很乱很抽象   
传统的4列5行秒解   
大概到了4列8行就很难穷举了。限制2个小兵可以穷举到4列10行   
   
##一些结论：
■ 小兵     ▅ ▅ 横块    ▋纵块     ▓ 2x2的块，“曹操”   

4x5的步数最多的局面是179/138步（一次移动一格是179，一次一个块移动多格是138），这关还有个名字“峰回路转”   
ShapeNum=2,3,4 BoardNum=109260 NoSolutionNum=25288  MaxStep = 179  Key = 15566407447498   
▋ ■■■   
▋ ▋ ▓ ▓   
▋ ▋ ▓ ▓   
▋ ▅ ▅   
▅ ▅ ■   


4x6的步数最多的局面是267/208步
ShapeNum=2,5,4 BoardNum=711660 MaxStep=267  Key=90487212424880   
  ▓ ▓ ■   
■▓ ▓ ■   
▋ ▅ ▅ ▋   
▋ ▅ ▅ ▋   
▋ ▋ ▋ ■   
▋ ▋ ▋   


4x8有个483步的局面（非严谨穷举，限制不超过4个小兵）   
ShapeNum=4,8,2 BoardNum=2680932 NoSolutionNum=1336757  MaxStep = 483  Key = 1109981651144150027_1250   
▅ ▅ ▓ ▓   
  ▋ ▓ ▓   
■▋ ▅ ▅   
▋ ▋ ▋ ▋   
▋ ▋ ▋ ▋   
■▅ ▅ ▋   
▋ ▋   ▋   
▋ ▋ ▅ ▅   

4x10有个615步的局面（非严谨穷举，限制2个小兵）   
ShapeNum=4,12,2 BoardNum=22218000 NoSolutionNum=9394054  MaxStep = 615  Key = 221921656011736315_7325250   
  ▋ ▅ ▅   
▋ ▋ ▓ ▓   
▋ ▋ ▓ ▓   
▋ ▋ ■▋   
▋ ▅ ▅ ▋   
▅ ▅ ▋ ▋   
▋   ▋ ▋   
▋ ▅ ▅ ■   
▋ ▋ ▋ ▋   
▋ ▋ ▋ ▋   
