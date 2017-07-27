// ������ DLL �ļ���

#include "stdafx.h"
#include "Connect.h"

void AttachHelper::Connect::OnAddInsUpdate(System::Array ^%custom)
{
}

void AttachHelper::Connect::OnBeginShutdown(System::Array ^%custom)
{
}

void AttachHelper::Connect::OnConnection(System::Object ^Application, ext_ConnectMode ConnectMode, System::Object ^AddInInst, System::Array ^%custom)
{
    _applicationObject = dynamic_cast<DTE2^>(Application);
    _addInInstance = dynamic_cast<AddIn^>(AddInInst);

    if(ConnectMode == ext_ConnectMode::ext_cm_UISetup)
    {
		SetUpUI();
    }
	else if (ConnectMode == ext_ConnectMode::ext_cm_Startup)
	{
		SetUiProperty();
	}
}

void AttachHelper::Connect::OnStartupComplete(System::Array ^%custom)
{
}

void AttachHelper::Connect::OnDisconnection(ext_DisconnectMode removeMode, System::Array ^%custom)
{
}


void AttachHelper::Connect::Exec(String ^CmdName, vsCommandExecOption ExecuteOption, Object ^%VariantIn, Object ^%VariantOut, bool %handled)
{
    handled = false;
    if(ExecuteOption == vsCommandExecOption::vsCommandExecOptionDoDefault)
    {
        if(!CmdName->CompareTo("AttachHelper.Connect.AttachHelper"))
        {
            handled = true;
            return;
        }
		if(!CmdName->CompareTo("AttachHelper.Connect.AttachPid"))
		{
			String ^strInput;
			if (VariantIn)
			{
				strInput = VariantIn->ToString();
				int iPid = Int32::Parse(strInput);
				Process2 ^stProces2;
				if (FindProcess(iPid, stProces2))
				{
					stProces2->Attach();
				}
				else
				{
					MessageBox::Show("PID:" + strInput + " Not Found.");
				}
			}
			else
			{

			}
			handled = true;
			return;
		}
		if(!CmdName->CompareTo("AttachHelper.Connect.DettachAll"))
		{
			Debugger2 ^debug2 = dynamic_cast<Debugger2^>(_applicationObject->Debugger);
			debug2->DetachAll();
			handled = true;
			return;
		}
    }
}

void AttachHelper::Connect::QueryStatus(String ^CmdName, vsCommandStatusTextWanted NeededText, vsCommandStatus %StatusOption, Object ^%CommandText)
{
    if(NeededText == vsCommandStatusTextWanted::vsCommandStatusTextWantedNone)
    {
        if(!CmdName->CompareTo("AttachHelper.Connect.AttachHelper")
			|| !CmdName->CompareTo("AttachHelper.Connect.AttachPid")
			|| !CmdName->CompareTo("AttachHelper.Connect.DettachAll"))
        {
            StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
            return;
        }
    }
}

bool AttachHelper::Connect::AddToolBar(_CommandBars ^ commandBarsOwner, String ^ToolBarName, CommandBar ^ %createdToolBar)
{
	try
	{
		createdToolBar = commandBarsOwner->Add(ToolBarName, MsoBarPosition::msoBarFloating, false, false);
	}
	catch (ArgumentException ^argumentException)
	{
		createdToolBar = commandBarsOwner[ToolBarName];
	}
	createdToolBar->Visible = true;
	return true;
}

bool AttachHelper::Connect::AddComboBox(CommandBar ^ ToolBarOwner,
										CommandBarComboBox ^ %cmdBarComboBox)
{
	try
	{
		CommandBarControl ^ cmdBarCtrl = ToolBarOwner->Controls->Add(MsoControlType::msoControlComboBox, Missing::Value, Missing::Value, Missing::Value, false);
		cmdBarComboBox = dynamic_cast<CommandBarComboBox ^>(cmdBarCtrl);
	}
	catch (Exception ^e)
	{
		MessageBox::Show(e->ToString());
	}

	return true;
}

bool AttachHelper::Connect::FindProcess(int iPid, Process2 ^ %stProces2)
{
	Debugger2 ^debug2 = dynamic_cast<Debugger2^>(_applicationObject->Debugger);
	Processes ^stProcesses = dynamic_cast<Processes^>(debug2->LocalProcesses);
	for (int iIndex = 1; iIndex <= stProcesses->Count; ++iIndex)
	{
		if (iPid == stProcesses->Item(iIndex)->ProcessID)
		{
			//MessageBox::Show("Name" + stProcesses->Item(iIndex)->Name);
			stProces2 = dynamic_cast<Process2^>(stProcesses->Item(iIndex));
			return true;
		}
	}
	return false;
}

bool AttachHelper::Connect::AddButton(CommandBar ^ FloatingToolBarOwner)
{
	throw gcnew System::NotImplementedException();
}

System::String ^ AttachHelper::Connect::GetToolsMenuName()
{
	String ^toolsMenuName;
	try
	{
		//��Ҫ���������ƶ�����һ���˵����򽫡����ߡ�һ�ʸ���Ϊ�˲˵���Ӣ�İ档
		//  �˴��뽫��ȡ�����ԣ�����׷�ӵ��˵����У�Ȼ�󽫴�������ӵ��ò˵��С�
		//  �����ڴ��ļ��п���ȫ�������˵����б�
		//  CommandBar.resx.
		ResourceManager ^resourceManager = gcnew ResourceManager("AttachHelper.CommandBar", Assembly::GetExecutingAssembly());
		CultureInfo ^cultureInfo = gcnew System::Globalization::CultureInfo(_applicationObject->LocaleID);
		if(cultureInfo->TwoLetterISOLanguageName == "zh")
		{
			CultureInfo ^parentCultureInfo = cultureInfo->Parent;
			toolsMenuName = resourceManager->GetString(String::Concat(parentCultureInfo->Name, "Tools"));
		}
		else
		{
			toolsMenuName = resourceManager->GetString(String::Concat(cultureInfo->TwoLetterISOLanguageName, "Tools"));
		}
	}
	catch(...)
	{
		//������ͼ���ҡ����ߡ�һ�ʵı��ػ��汾����δ���ҵ���
		//  Ĭ��ֵΪ en-US ���ʣ���ֵ���������ڵ�ǰ�����ԡ�
		toolsMenuName = "Tools";
	}
	return toolsMenuName;
}

void AttachHelper::Connect::GetUiHandule()
{
	_CommandBars ^commandBars = dynamic_cast<CommandBars^>(_applicationObject->CommandBars);
	m_attachHelperFloatingToolBar = commandBars[m_strToolBarName];
	m_cmdBarComboBox = dynamic_cast<CommandBarComboBox ^>(
		m_attachHelperFloatingToolBar->Controls[m_strComboBoxCaption]);
}

void AttachHelper::Connect::SetUiProperty()
{
	GetUiHandule();
	SetComboBoxProperty(m_cmdBarComboBox, m_strComboBoxCaption
		, gcnew _CommandBarComboBoxEvents_ChangeEventHandler(ComboBoxEventHandle));
}

void AttachHelper::Connect::SetComboBoxProperty(CommandBarComboBox ^ cmdBarComboBox, String ^strComboBoxCaption,
						 _CommandBarComboBoxEvents_ChangeEventHandler ^eventHandle)
{
	if (!cmdBarComboBox)
	{
		return ;
	}
	try
	{
		cmdBarComboBox->Caption = strComboBoxCaption;
		cmdBarComboBox->DropDownLines = 6;
		cmdBarComboBox->DropDownWidth = 100;
		cmdBarComboBox->Visible = true;
		cmdBarComboBox->Change += eventHandle;
	}
	catch (Exception ^e)
	{
		MessageBox::Show(e->ToString());
	}

}

void AttachHelper::Connect::SetUpUI()
{
	array< Object^ >^ contextGUIDs = gcnew array< Object^ >(0);
	Commands2 ^commands = dynamic_cast<Commands2^>(_applicationObject->Commands);
	String ^toolsMenuName = GetToolsMenuName();
	//�����������ڡ����ߡ��˵��ϡ�
	//���� MenuBar �����������������������������˵���Ķ���������:
	_CommandBars ^commandBars = dynamic_cast<CommandBars^>(_applicationObject->CommandBars);
	CommandBar ^menuBarCommandBar = dynamic_cast<CommandBar^>(commandBars["MenuBar"]);

	//�� MenuBar �������ϲ��ҡ����ߡ�������:
	CommandBarControl ^toolsControl = menuBarCommandBar->Controls[toolsMenuName];
	CommandBarPopup ^toolsPopup = dynamic_cast<CommandBarPopup^>(toolsControl);

	//���ϣ����Ӷ����������ӳ��������������ظ��� try/catch �飬
	//  ֻ��ȷ������ QueryStatus/Exec ������ʹ������µ���������
	try
	{	
		AddToolBar(commandBars, m_strToolBarName, m_attachHelperFloatingToolBar);

		try
		{
			//��һ��������ӵ� Commands ����:
			Command ^command = commands->AddNamedCommand2(_addInInstance, "AttachHelper", "AttachHelper", "Executes the command for AttachHelper", true, 59, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			// ���������ӽ��̣� ���: ���̵�pid
			Command ^commandAttachPid = commands->AddNamedCommand2(_addInInstance, "AttachPid", "AttachPid", "����PID���ӽ���", true, 59, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			// �������������н���
			Command ^commandDettachAll = commands->AddNamedCommand2(_addInInstance, "DettachAll", "DettachAll", "�������н���", true, 60, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePict, vsCommandControlType::vsCommandControlTypeButton);


			//����Ӧ�ڸ�����Ŀؼ���ӵ�������ӵ�������������:
			if((commandDettachAll) && (m_attachHelperFloatingToolBar))
			{
				commandDettachAll->AddControl(m_attachHelperFloatingToolBar, 1);
			}
		}
		catch(Exception ^exceptioninfo)
		{

		}
		try
		{
			m_attachHelperFloatingToolBar->Controls[m_strComboBoxCaption];
		}
		catch (...)
		{
			AddComboBox(m_attachHelperFloatingToolBar, m_cmdBarComboBox);
			SetComboBoxProperty(m_cmdBarComboBox, m_strComboBoxCaption
				, gcnew _CommandBarComboBoxEvents_ChangeEventHandler(ComboBoxEventHandle));
		}

	}
	catch(System::ArgumentException ^argumentException)
	{
		//������ִ��쳣��ԭ��ܿ��������ھ��и����Ƶ�����
		//  �Ѵ��ڡ����ȷʵ��ˣ����������´������������
		//  ���Է��ĺ��Դ��쳣��
	}
	catch(Exception ^exceptioninfo)
	{

	}
}

//
System::Void AttachHelper::ComboBoxEventHandle(Microsoft::VisualStudio::CommandBars::CommandBarComboBox^ Ctrl)
{
	String ^strComboBoxSelectText = Ctrl->Text;
	if (0 == strComboBoxSelectText->Length)
	{
		// ����Ϊ�գ���Ч�¼���ֱ�ӷ���
		return;
	}

	DTE2^ _applicationObject = dynamic_cast<DTE2^>(Ctrl->Application);
	
	int iInputProcessID = 0;
	try
	{
		iInputProcessID = Int32::Parse(strComboBoxSelectText);
	}
	catch (Exception ^e)
	{
		MessageBox::Show("��������"+ e->ToString());
	}

	if (0 < iInputProcessID)
	{
		try
		{
			_applicationObject->ExecuteCommand("AttachHelper.Connect.AttachPid", strComboBoxSelectText);
			bool bFound = false;
			for (int iIndex = 1; iIndex <= Ctrl->ListCount; ++iIndex)
			{
				if (strComboBoxSelectText == Ctrl->List[iIndex])
				{
					bFound = true;
				}
			}
			// List���޴���
			if (!bFound)
			{
				Ctrl->AddItem(strComboBoxSelectText, Missing::Value);
			}
		}
		catch (Exception ^e)
		{
			MessageBox::Show(e->ToString());
		}
	}
	Ctrl->Text = ""; // ������룬�����󴥷�
}
