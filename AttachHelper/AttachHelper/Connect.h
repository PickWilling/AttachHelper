#pragma once

using namespace System;
using namespace Extensibility;
using namespace EnvDTE;
using namespace EnvDTE80;
using namespace Microsoft::VisualStudio::CommandBars;
using namespace System::Resources;
using namespace System::Reflection;
using namespace System::Globalization;
using namespace System::Windows::Forms;

namespace AttachHelper
{
	/// <summary>����ʵ����ӳ���Ķ���</summary>
	/// <seealso class='IDTExtensibility2' />
	public ref class Connect : public IDTExtensibility2, public IDTCommandTarget
	{

	public:
		/// <summary>ʵ����ӳ������Ĺ��캯�����뽫���ĳ�ʼ���������ڴ˷����ڡ�</summary>
		Connect()
		{
		}

		/// <summary>ʵ�� IDTExtensibility2 �ӿڵ� OnAddInsUpdate ����������ӳ��򼯺��ѷ�������ʱ����֪ͨ��</summary>
		/// <param term='custom'>�ض�������Ӧ�ó���Ĳ������顣</param>
		/// <seealso class='IDTExtensibility2' />	
		virtual void OnAddInsUpdate(System::Array ^%custom);

		/// <summary>ʵ�� IDTExtensibility2 �ӿڵ� OnBeginShutdown ��������������ж������Ӧ�ó����֪ͨ��</summary>
		/// <param term='custom'>�ض�������Ӧ�ó���Ĳ������顣</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnBeginShutdown(System::Array ^%custom);

		/// <summary>ʵ�� IDTExtensibility2 �ӿڵ� OnConnection �������������ڼ�����ӳ����֪ͨ��</summary>
		/// <param term='application'>����Ӧ�ó���ĸ�����</param>
		/// <param term='connectMode'>������ӳ���ļ��ط�ʽ��</param>
		/// <param term='addInInst'>��ʾ����ӳ���Ķ���</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnConnection(System::Object ^Application, ext_ConnectMode ConnectMode, System::Object ^AddInInst, System::Array ^%custom);

		/// <summary>ʵ�� IDTExtensibility2 �ӿڵ� OnStartupComplete ��������������Ӧ�ó�������ɼ��ص�֪ͨ��</summary>
		/// <param term='custom'>�ض�������Ӧ�ó���Ĳ������顣</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnStartupComplete(System::Array ^%custom);

		/// <summary>ʵ�� IDTExtensibility2 �ӿڵ� OnDisconnection ��������������ж����ӳ����֪ͨ��</summary>
		/// <param term='disconnectMode'>������ӳ����ж�ط�ʽ��</param>
		/// <param term='custom'>�ض�������Ӧ�ó���Ĳ������顣</param>
		/// <seealso class='IDTExtensibility2' />
		virtual void OnDisconnection(ext_DisconnectMode removeMode, System::Array ^%custom);

		/// <summary>ʵ�� IDTCommandTarget �ӿڵ� Exec �������˷����ڵ��ø�����ʱ���á�</summary>
		/// <param term='commandName'>Ҫִ�е���������ơ�</param>
		/// <param term='executeOption'>����������Ӧ������С�</param>
		/// <param term='varIn'>�ӵ��÷����ݵ���������Ĳ�����</param>
		/// <param term='varOut'>���������򴫵ݵ����÷��Ĳ�����</param>
		/// <param term='handled'>֪ͨ���÷��������Ƿ��ѱ�����</param>
		/// <seealso class='Exec' />
		virtual void Exec(String ^CmdName, vsCommandExecOption ExecuteOption, Object ^%VariantIn, Object ^%VariantOut, bool %handled);

		/// <summary>ʵ�� IDTCommandTarget �ӿڵ� QueryStatus �������˷����ڸ��¸�����Ŀ�����ʱ����</summary>
		/// <param term='commandName'>Ҫȷ����״̬����������ơ�</param>
		/// <param term='neededText'>������������ı���</param>
		/// <param term='status'>���������û������е�״̬��</param>
		/// <param term='commandText'>neededText ������Ҫ����ı���</param>
		/// <seealso class='Exec' />
		virtual void QueryStatus(String ^CmdName, vsCommandStatusTextWanted NeededText, vsCommandStatus %StatusOption, Object ^%CommandText);

	private:
		DTE2 ^_applicationObject;
		AddIn ^_addInInstance;
		bool AddToolBar(_CommandBars ^ commandBarsOwner, String ^ToolBarName, CommandBar ^ %createdToolBar);
		bool AddComboBox(CommandBar ^ ToolBarOwner, _CommandBarComboBoxEvents_ChangeEventHandler ^eventHandle, String ^strComboBoxCaption);
		bool FindProcess(int iPid, Process2 ^ stProces2);
		bool AddButton(CommandBar ^ FloatingToolBarOwner);
	};
	// ComboBox �����¼�
	Void ComboBoxEventHandle(Microsoft::VisualStudio::CommandBars::CommandBarComboBox^ Ctrl);
}
