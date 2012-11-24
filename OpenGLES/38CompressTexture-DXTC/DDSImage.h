// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DDSIMAGE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DDSIMAGE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DDSIMAGE_EXPORTS
#define DDSIMAGE_API __declspec(dllexport)
#else
#define DDSIMAGE_API __declspec(dllimport)
#endif

/** DDS纹理绑定函数
 * @param[in] filePath 纹理路径
 * @param[out] texID 纹理ID
 * @return 数据信息
 * - 0 打开失败
 * - 1 RGB格式
 * - 2 RGBA格式
 */

extern "C" DDSIMAGE_API unsigned BuildDDS(char *filePath, unsigned &texID);
