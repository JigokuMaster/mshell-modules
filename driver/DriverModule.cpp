
#include "NativeModule.h"
#include "TextWriter.h"
#include <e32cmn.h>



_LIT(KAboutString, "DriverModule (C) 2025 - JigokuMaster");

class CDriverChannnel : public RBusLogicalChannel
{
	
public:
	enum ChannelState { RCBusOpen, RCBusClosed };

        inline TInt Open(TDesC& aLddName, const TVersion& aVer, TBool aThreadOwner)
		{
        	if(aThreadOwner)
			{
				return DoCreate(aLddName, aVer, KNullUnit, NULL, NULL, EOwnerThread);
			}
        	return DoCreate(aLddName, aVer, KNullUnit, NULL, NULL);
        }
		
        inline TInt SendControl(TInt aReq, TAny* aArg0, TAny* aArg1)
        {
        	return DoControl(aReq, aArg0 , aArg1);
        }
};

class DriverModule : public NativeModule {
private:
  // the functions this module supports,
  // the enumeration values correspond to function indices
  enum Functions {
    GetLoadedLDDsFunction, LoadLDDFunction,
	UnLoadLDDFunction, OpenChannelFunction,
	CloseChannelFunction, DoControlFunction,
	AboutFunction, TestFunction
  };

  ~DriverModule()
  {
	  CloseChannel();
  }
  
private:
	CDriverChannnel iDriverChannnel;
	TInt iDriverChannnelState;

private:
	Runtime::Value GetLoadedLDDsL()
	{
		Runtime::Value result;
		Runtime::Array *array;
		result = runtime->NewArrayL(array, 30);
		TFindLogicalDevice findLogicalDevice;
		TFullName name;
		while (findLogicalDevice.Next(name) == KErrNone)
		{
			array->AppendL( runtime->NewStringL(name));			
		}
		return result;
	}

	Runtime::Value OpenChannelL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TPtrC aLddName;
		TVersion aVer(1,0,0);
		TBool aThreadOwner = EFalse;

		if(iDriverChannnelState == CDriverChannnel::RCBusOpen)
		{
			runtime->GetWriter()->WriteL(_L("driver channel is already open, you must call close_channel() first\n"));
			result.SetNumber(KErrArgument);
			return result;
		}
		
		if(paramCount == 1)
		{
			if(!params[0].IsNull() && params[0].IsString())
			{
				aLddName.Set(params[0].GetPtrCL());
			}
			else
			{
				runtime->GetWriter()->WriteL(_L("1st argument must be string\n"));
				result.SetNumber(KErrArgument);
				return result;
			}	
		}
		
		if(paramCount == 2)
		{
			
			if(!params[1].IsNull() && params[1].IsArray())
			{
				Runtime::Array arr = params[1].GetArrayL();
				if(arr.Length() == 3)
				{
					aVer.iMajor = arr.GetL(0).GetIntL();
					aVer.iMinor = arr.GetL(1).GetIntL();
					aVer.iBuild = arr.GetL(2).GetIntL();
				}
				else
				{
					runtime->GetWriter()->WriteL(_L("no enough items for version argument\n"));
					result.SetNumber(KErrArgument);
					return result;
				}
			}
			else
			{
				runtime->GetWriter()->WriteL(_L("2nd argument must be array\n"));
				result.SetNumber(KErrArgument);
				return result;
			}	

		}
		
		if(paramCount == 3)
		{
			if(!params[2].IsNull() && params[2].IsBoolean())
			{
				aThreadOwner = params[2].GetBooleanL();
			}
			else
			{
				runtime->GetWriter()->WriteL(_L("3rd argument must be boolean\n"));
				result.SetNumber(KErrArgument);
				return result;
			}	

		}

		TInt err = iDriverChannnel.Open(aLddName, aVer, aThreadOwner);
		if(err == KErrNone)
		{
			iDriverChannnelState = CDriverChannnel::RCBusOpen;
		}	
		result.SetNumber(err);
		return result;
	}

	void CloseChannel()
	{
		if((iDriverChannnelState == CDriverChannnel::RCBusOpen) && iDriverChannnel.Handle())
		{
			iDriverChannnelState = CDriverChannnel::RCBusClosed;
			iDriverChannnel.Close();
		}
	}
	
	Runtime::Value ChannelDoControlL(Runtime::Value *params, TInt paramCount)
	{
		Runtime::Value result;
		TInt err = KErrNone;
		TInt aFunc;
		TAny* a1 = NULL;
		TAny* a2 = NULL;

		if(iDriverChannnelState == CDriverChannnel::RCBusOpen)
		{
			if(!params[0].IsNull() && params[0].IsNumber())
			{
				aFunc = params[0].GetIntL();
			}
			else
			{
				runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
				result.SetNumber(KErrArgument);
				return result;
			}
			
			if(paramCount == 2)
			{
				if(!params[1].IsNull() && params[1].IsNumber())
				{
					a1 = (TAny*)params[1].GetIntL();
				}
				else if(!params[1].IsNull() && params[1].IsString())
				{
					a1 = (TAny*)( params[1].GetPtrCL().Ptr() );
				}

			}	

			if(paramCount == 3)
			{
				if(!params[2].IsNull() && params[2].IsNumber())
				{
					a2 = (TAny*)params[2].GetIntL();
				}
				else if(!params[2].IsNull() && params[2].IsString())
				{
					a2 = (TAny*)( params[2].GetPtrCL().Ptr() );
				}
			}	
			
		}
		
		else
		{
			runtime->GetWriter()->WriteL(_L("open_channel() must be called before do_control()\n"));
			result.SetNumber(KErrArgument);
			return result;
		}
		
		err = iDriverChannnel.SendControl(aFunc, a1, a2);
		result.SetNumber(err);
		return result;
	}

	Runtime::Value LoadLdd(Runtime::Value *params, TInt paramCount)
	{
		Runtime::Value result;
		result.SetNumber(KErrNone);
		if(!params[0].IsNull() && params[0].IsString())
		{
				TInt err = User::LoadLogicalDevice(params[0].GetPtrCL());
				if ((err != KErrNone) && ( err != KErrAlreadyExists))
				{
					result.SetNumber(err);
				}
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("argument must be string\n"));
			result.SetNumber(KErrArgument);
		}
		return result;
	}

	Runtime::Value UnLoadLdd(Runtime::Value *params, TInt paramCount)
	{
		Runtime::Value result;
		result.SetNumber(KErrNone);
		if(!params[0].IsNull() && params[0].IsString())
		{
				TInt err = User::FreeLogicalDevice(params[0].GetPtrCL());
				if (err != KErrNone)
				{
					result.SetNumber(err);
				}
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("argument must be string\n"));
			result.SetNumber(KErrArgument);
		}
		return result;
	}

	Runtime::Value TestParamsL(Runtime::Value *params, TInt paramCount)
	{
		Runtime::Value result;
		result.SetNumber(KErrNone);
		if(!params[0].IsNull() && params[0].IsArray())
		{
			runtime->GetWriter()->WriteL(_L("array items:\n"));
			Runtime::Array arr = params[0].GetArrayL();
			for(TInt i = 0; i < arr.Length(); i++)
			{
				runtime->WriteL(arr.GetL(i));
				runtime->GetWriter()->WriteL(_L("\n"));
			}
		}
		return result;
	}


protected: // from NativeModule
  TInt ExpectedRuntimeVersion() { return Runtime::VERSION; }

  const char *ModuleVersion() { return "$Revision: 1.0 $"; }

  void ConstructL() {
    // the functions this module supports
    runtime->AddNativeFunctionL(_L("loaded_ldds"), 0, 0, GetLoadedLDDsFunction);
    runtime->AddNativeFunctionL(_L("load_ldd"), 1, 1, LoadLDDFunction);
    runtime->AddNativeFunctionL(_L("unload_ldd"), 1, 1, UnLoadLDDFunction);
    runtime->AddNativeFunctionL(_L("open_channel"), 1, 5, OpenChannelFunction);
    runtime->AddNativeFunctionL(_L("close_channel"), 0, 0, CloseChannelFunction);
    runtime->AddNativeFunctionL(_L("do_control"), 1, 3, DoControlFunction);
    runtime->AddNativeFunctionL(_L("about"), 0, 0, AboutFunction);
    runtime->AddNativeFunctionL(_L("test"), 1, 1, TestFunction);
	iDriverChannnelState = CDriverChannnel::RCBusClosed;
	
  }
  
  Runtime::Value ExecuteL(TInt index, Runtime::Value *params,
                          TInt paramCount, TRequestStatus &status)
 {

    // a variable which will contain the function result
    Runtime::Value result;
    result.SetNull(); // default for functions returning null
    switch (index)
	{
		case GetLoadedLDDsFunction:
		{
			result = GetLoadedLDDsL();
			break;
		}
		case LoadLDDFunction:
		{
			result = LoadLdd(params, paramCount);
			break;
		}

		case UnLoadLDDFunction:
		{
			result = UnLoadLdd(params, paramCount);
			break;
		}

		case OpenChannelFunction:
		{
			result = OpenChannelL(params, paramCount);
			break;
		}

		case CloseChannelFunction:
		{
			CloseChannel();
			break;
		}

		case DoControlFunction:
		{
			result = ChannelDoControlL(params, paramCount);
			break;
		}

		case AboutFunction:
		{
			result = runtime->NewStringL(KAboutString);
			break;
		}
		case TestFunction:
		{
			result = TestParamsL(params, paramCount);
			break;
		}

    }
    return result;
  }

};

// the first exported function creates an instance of this module
EXPORT_C NativeModule* NewDriverModuleL() { return new(ELeave) DriverModule; }

// the second exported function is required by DLL conventions (2nd ed only)
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason) { return KErrNone; }
#endif
