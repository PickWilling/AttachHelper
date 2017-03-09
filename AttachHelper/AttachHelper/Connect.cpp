// 这是主 DLL 文件。

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

    if(ConnectMode == ext_ConnectMode::ext_cm_UISetup || ConnectMode == ext_ConnectMode::ext_cm_Startup)
    {
        array< Object^ >^ contextGUIDs = gcnew array< Object^ >(0);
        Commands2 ^commands = dynamic_cast<Commands2^>(_applicationObject->Commands);
        String ^toolsMenuName;

        try
        {
            //若要将此命令移动到另一个菜单，则将“工具”一词更改为此菜单的英文版。
            //  此代码将获取区域性，将其追加到菜单名中，然后将此命令添加到该菜单中。
            //  您会在此文件中看到全部顶级菜单的列表
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
            //我们试图查找“工具”一词的本地化版本，但未能找到。
            //  默认值为 en-US 单词，该值可能适用于当前区域性。
            toolsMenuName = "Tools";
        }

        //将此命令置于“工具”菜单上。
        //查找 MenuBar 命令栏，该命令栏是容纳所有主菜单项的顶级命令栏:
        _CommandBars ^commandBars = dynamic_cast<CommandBars^>(_applicationObject->CommandBars);
        CommandBar ^menuBarCommandBar = dynamic_cast<CommandBar^>(commandBars["MenuBar"]);

        //在 MenuBar 命令栏上查找“工具”命令栏:
        CommandBarControl ^toolsControl = menuBarCommandBar->Controls[toolsMenuName];
        CommandBarPopup ^toolsPopup = dynamic_cast<CommandBarPopup^>(toolsControl);

        //如果希望添加多个由您的外接程序处理的命令，可以重复此 try/catch 块，
        //  只需确保更新 QueryStatus/Exec 方法，使其包含新的命令名。
        try
        {	
			CommandBar ^attachHelperFloatingToolBar;
			AddToolBar(commandBars, "AttachHelper", attachHelperFloatingToolBar);
			if (ConnectMode != ext_ConnectMode::ext_cm_Startup)
			{
				//将一个命令添加到 Commands 集合:
				Command ^command = commands->AddNamedCommand2(_addInInstance, "AttachHelper", "AttachHelper", "Executes the command for AttachHelper", true, 59, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
				// 添加命令：附加进程， 入参: 进程的pid
				Command ^commandAttachPid = commands->AddNamedCommand2(_addInInstance, "AttachPid", "AttachPid", "根据PID附加进程", true, 59, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
				// 添加命令：分离所有进程
				Command ^commandDettachAll = commands->AddNamedCommand2(_addInInstance, "DettachAll", "DettachAll", "分离所有进程", true, 60, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePict, vsCommandControlType::vsCommandControlTypeButton);


				//将对应于该命令的控件添加到“新添加到浮动工具栏”:
				if((commandDettachAll) && (attachHelperFloatingToolBar))
				{
					commandDettachAll->AddControl(attachHelperFloatingToolBar, 1);
				}
			}
			else
			{
				AddComboBox(attachHelperFloatingToolBar, 
					gcnew _CommandBarComboBoxEvents_ChangeEventHandler(ComboBoxEventHandle));
			}
			
        }
		catch(System::ArgumentException ^argumentException)
        {
			//如果出现此异常，原因很可能是由于具有该名称的命令
			//  已存在。如果确实如此，则无需重新创建此命令，并且
            //  可以放心忽略此异常。
        }
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

bool AttachHelper::Connect::AddComboBox(CommandBar ^ ToolBarOwner, _CommandBarComboBoxEvents_ChangeEventHandler ^eventHandle)
{
	try
	{
		CommandBarControl ^ cmdBarCtrl = ToolBarOwner->Controls->Add(MsoControlType::msoControlComboBox, Missing::Value, Missing::Value, Missing::Value, false);
		CommandBarComboBox ^ cmdBarComboBox = dynamic_cast<CommandBarComboBox ^>(cmdBarCtrl);
		cmdBarComboBox->DropDownLines = 6;
		cmdBarComboBox->DropDownWidth = 100;
		cmdBarComboBox->Visible = true;
		cmdBarComboBox->Change += eventHandle;
	}
	catch (Exception ^e)
	{
		MessageBox::Show(e->ToString());
	}

	return true;
}

bool AttachHelper::Connect::FindProcess(int iPid, Process2 ^ stProces2)
{
	Debugger2 ^debug2 = dynamic_cast<Debugger2^>(_applicationObject->Debugger);
	Processes ^stProcesses = dynamic_cast<Processes^>(debug2->LocalProcesses);
	for (int iIndex = 1; iIndex < stProcesses->Count; ++iIndex)
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

//
System::Void AttachHelper::ComboBoxEventHandle(Microsoft::VisualStudio::CommandBars::CommandBarComboBox^ Ctrl)
{
	String ^strComboBoxSelectText = Ctrl->Text;
	if (0 == strComboBoxSelectText->Length)
	{
		// 输入为空，无效事件，直接返回
		return ;
	}

	DTE2^ _applicationObject = dynamic_cast<DTE2^>(Ctrl->Application);
	
	int iInputProcessID = 0;
	try
	{
		iInputProcessID = Int32::Parse(strComboBoxSelectText);
	}
	catch (Exception ^e)
	{
		MessageBox::Show("输入有误："+ e->ToString());
	}

	if (0 < iInputProcessID)
	{
		try
		{
			_applicationObject->ExecuteCommand("AttachHelper.Connect.AttachPid", strComboBoxSelectText);
		}
		catch (Exception ^e)
		{
			MessageBox::Show(e->ToString());
		}

		bool bFound = false;
		for (int iIndex = 0; iIndex < Ctrl->ListCount; ++iIndex)
		{
			if (strComboBoxSelectText == Ctrl->List[iIndex])
			{
				bFound = true;
			}
		}
		// List中无此项
		if (!bFound)
		{
			Ctrl->AddItem(strComboBoxSelectText, Missing::Value);
		}
	}
}
