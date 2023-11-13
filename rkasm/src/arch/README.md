# ISA

命令は、アセンブリの「文字列」表現と、「バイナリ」表現の２通りがあります。

理想的には 1 - 1 の関係になりますが、実用上は疑似命令の存在により、N - 1 の関係になります。

```
'string' → inst(rs1 rs2 rd imm) → cpu(opc func rs1 rs2 rd) → 'binary'
```

アセンブリの命令一覧は `isa.hpp` で定義されており、`format.hpp` で定義されるフォーマットに従っています。

## 命令の変換規則

```cpp:
int reg_to_int(string);

template <int n>
struct BinMap{
    std::array<n, int> map;
    std::array<n, TmpVar | int> content;
};

struct TmpVar{
    std::string key;
    std::function<int(std::string)> str_to_int;
    std::function<std::string(int)> int_to_str;
};

auto rd = TmpVar{"rd", reg_to_int, int_to_reg};

auto imm = TmpVar{""};

auto mapA = {4,4,4,4,16};

auto isa = std::vector<InsnFormat>{
    {"add", {rd, rs1, rs2}, {mapA,{OPC::calc, rs1, rs2, rd, Func::ADD}}}
};
```

最初のフィールド `"add"` は、命令のニーモニックを指定しています。

次のフィールドは、命令の引数を指定します。
引数の名前と、文字列を数値に変換する関数のペアを指定します。

最後のフィールドは、バイナリ表現を記述しています。
数値または文字列と、ビット長を表した数値のペアで記述されます。
数値は、そのまま出力されます。
文字列は、対応する引数を整数に変換してからそこに配置することを意味します。
