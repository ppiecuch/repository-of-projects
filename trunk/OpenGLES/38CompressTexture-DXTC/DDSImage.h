// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DDSIMAGE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DDSIMAGE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef DDSIMAGE_EXPORTS
#define DDSIMAGE_API __declspec(dllexport)
#else
#define DDSIMAGE_API __declspec(dllimport)
#endif

/** DDS����󶨺���
 * @param[in] filePath ����·��
 * @param[out] texID ����ID
 * @return ������Ϣ
 * - 0 ��ʧ��
 * - 1 RGB��ʽ
 * - 2 RGBA��ʽ
 */

extern "C" DDSIMAGE_API unsigned BuildDDS(char *filePath, unsigned &texID);
