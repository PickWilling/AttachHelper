#include "stdafx.h"

using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;

//
// �йس��򼯵ĳ�����Ϣͨ���������Լ�
// ���ơ�������Щ����ֵ���޸�
// ����򼯹�������Ϣ��
//
[assembly:AssemblyTitleAttribute("")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("")];
[assembly:AssemblyCopyrightAttribute("")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];		

//
// ���򼯵İ汾��Ϣ�������ĸ�ֵ���:
//
//      ���汾
//      �ΰ汾
//      �ڲ��汾��
//      �޶���
//
// ��ָ��������Щֵ��Ҳ����ʹ�á��޶��š��͡��ڲ��汾�š���Ĭ��ֵ��
// �����ǰ�������ʾʹ�á�*��:

[assembly:AssemblyVersionAttribute("1.0.*")];

//
// Ҫ�Գ��򼯽���ǩ��������ָ��Ҫʹ�õ���Կ���йس���ǩ���ĸ�����Ϣ����ο�
// Microsoft .NET Framework �ĵ���
//
// ʹ����������Կ�������ǩ������Կ��
//
// ע��:
//   (*) ���δָ���κ���Կ���򲻶Գ���ǩ����
//   (*) KeyName ��ָ������ϵļ��ܷ���
//       �ṩ����(CSP)���Ѿ���װ����Կ��KeyFile ��ָ������Կ��
//       �ļ���
//   (*) ��� KeyFile �� KeyName ֵ����ָ������
//       �����·�ʽ���д���:
//       (1) ������� CSP ���ҵ� KeyName����ʹ�ø���Կ��
//       (2) ��� KeyName �����ڶ� KeyFile ���ڣ�
//           ���� CSP �а�װ��ʹ�� KeyFile �е���Կ��
//   (*) Ϊ�˴��� KeyFile��������ʹ�� sn.exe (ǿ����)ʵ�ù��ߡ�
//        ָ�� KeyFile ʱ��KeyFile ��λ��Ӧ��
//        ���������ĿĿ¼�����·����
//   (*) ���ӳ�ǩ������һ���߼�ѡ�� - �й����ĸ�����Ϣ��
//       ����� Microsoft .NET Framework �ĵ���
//
[assembly:AssemblyDelaySignAttribute(false)];
[assembly:AssemblyKeyFileAttribute("")];
[assembly:AssemblyKeyNameAttribute("")];

[assembly:ComVisible(true)];