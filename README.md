# cstr
A lightweight dynamic string library for C.
一个轻量级的 C 语言动态字符串库。

## API Reference | 函数接口

| Function | Description | 说明 |
| :--- | :--- | :--- |
| `int cstr_init(cstr* dst)` | Initialize string object. | 初始化字符串对象。 |
| `cstr* cstr_create_p(void)` | Create & init string on heap. | 在堆上创建并初始化字符串。 |
| `int cstr_dstr_p(cstr* dst)` | Destroy heap-allocated string. | 销毁堆分配的字符串。 |
| `int cstr_reset(cstr* dst)` | Free buffer and reset state. | 释放内存并重置状态。 |
| `int cstr_clear(cstr* dst)` | Clear content, keep capacity. | 清空内容，保留容量。 |
| `int cstr_set(cstr* dst, const char* src)` | Set new content. | 设置新内容。 |
| `int cstr_append(cstr* dst, const char* src)` | Append string. | 追加字符串。 |
| `int cstr_insert(cstr* dst, const char* src, size_t pos)` | Insert at position. | 在指定位置插入。 |
| `int cstr_remove(cstr* dst, size_t pos, size_t len)` | Remove substring. | 删除指定区间。 |
| `int cstr_setcap(cstr* dst, size_t cap)` | Adjust buffer capacity. | 调整缓冲区容量。 |
| `int cstr_setcap_unsafe(cstr* dst, size_t cap)` | Adjust capacity (no safety checks). | 调整容量（无安全检查）。 |
| `int cstr_shrink(cstr* dst)` | Shrink buffer to fit. | 收缩内存至合适大小。 |

## Return Values | 返回值
*   **CSTR_OK (0)**: Success | 成功
*   **CSTR_NOMEN (1)**: Out of memory | 内存不足
*   **CSTR_INVARG (2)**: Invalid argument | 参数无效

## License | 许可证
MIT
