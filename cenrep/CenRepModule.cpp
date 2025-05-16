
#include "NativeModule.h"
#include "TextWriter.h"
#include <centralrepository.h>


_LIT(KAboutString, "CenRepModule (C) 2025 - JigokuMaster");

class CenRepModule : public NativeModule {
private:
  // the functions this module supports,
  // the enumeration values correspond to function indices
  enum Functions {
    GetIntFunction, GetStringFunction,
    SetIntFunction, SetStringFunction,
    CreateIntFunction, CreateStringFunction,
	DeleteFunction, AboutFunction
  };

  ~CenRepModule()
  {
  }

private:
	Runtime::Value GetIntL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		TInt aValue = -1;

		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			error = rep->Get(aKey, aValue);
			if(error == KErrNone)
			{
				result.SetNumber(aValue);
				delete rep;
				return result;
			}
		}
		
		if(rep)
		{
			delete rep;
		}	
		result.SetNumber(error);
		return result;
	}

	Runtime::Value SetIntL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		TInt aValue = -1;

		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}

		if(!params[2].IsNull() && params[2].IsNumber())
		{
			aValue = params[2].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("3rd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			error = rep->Set(aKey, aValue);
		}
		
		if(rep)
		{
			delete rep;
		}	
		result.SetNumber(error);
		return result;
	}


	Runtime::Value GetStringL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		TBuf<1024+1> aValue;
		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			error = rep->Get(aKey, aValue);
			if(error == KErrNone)
			{
				aValue.Append(0);
				result = runtime->NewStringL(aValue);
				delete rep;
				return result;
			}
		}
		
		if(rep)
		{
			delete rep;
		}
		
		result.SetNumber(error);		
		return result;
	}

	Runtime::Value SetStringL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			if(!params[2].IsNull() && params[2].IsString())
			{
				error = rep->Set(aKey, params[2].GetPtrCL());
			}

			else
			{
				runtime->GetWriter()->WriteL(_L("3rd argument must be string\n"));
				error = KErrArgument;
			}

		}

		if(rep)
		{
			delete rep;
		}
		
		result.SetNumber(error);		
		return result;
	}

	Runtime::Value CreateIntL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		TInt aValue = -1;

		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}

		if(!params[2].IsNull() && params[2].IsNumber())
		{
			aValue = params[2].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("3rd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			error = rep->Create(aKey, aValue);
		}
		
		if(rep)
		{
			delete rep;
		}	
		result.SetNumber(error);
		return result;
	}

	Runtime::Value CreateStringL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}
		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			if(!params[2].IsNull() && params[2].IsString())
			{
				error = rep->Create(aKey, params[2].GetPtrCL());
			}

			else
			{
				runtime->GetWriter()->WriteL(_L("3rd argument must be string\n"));
				error = KErrArgument;
			}

		}

		if(rep)
		{
			delete rep;
		}
		
		result.SetNumber(error);		
		return result;
	}


	Runtime::Value DeleteRepL(Runtime::Value *params, TInt paramCount)
	{

		Runtime::Value result;
		TInt error = KErrNone;
		CRepository* rep = NULL;
		TInt aUid = -1;
		TUint32 aKey = 0;
		if(!params[0].IsNull() && params[0].IsNumber())
		{
			aUid = params[0].GetIntL();
		}
		else
		{
			runtime->GetWriter()->WriteL(_L("1st argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;		
		}

		if(!params[1].IsNull() && params[1].IsNumber())
		{
			aKey = params[1].GetIntL();
		}

		else
		{
			runtime->GetWriter()->WriteL(_L("2nd argument must be int\n"));
			result.SetNumber(KErrArgument);
			return result;					
		}

		
		TRAP(error, rep = CRepository::NewL(TUid::Uid(aUid)) );
		if(error == KErrNone)
		{
			error = rep->Delete(aKey);
		}
		
		if(rep)
		{
			delete rep;
		}	
		result.SetNumber(error);
		return result;
	}




protected: // from NativeModule
  TInt ExpectedRuntimeVersion() { return Runtime::VERSION; }

  const char *ModuleVersion() { return "$Revision: 1.0 $"; }

  void ConstructL() {
    // the functions this module supports
    runtime->AddNativeFunctionL(_L("get_int"), 2, 2, GetIntFunction);
    runtime->AddNativeFunctionL(_L("get_str"), 2, 2, GetStringFunction);
    runtime->AddNativeFunctionL(_L("set_int"), 3, 3, SetIntFunction);
    runtime->AddNativeFunctionL(_L("set_str"), 3, 3, SetStringFunction);
    runtime->AddNativeFunctionL(_L("create_int"), 3, 3, CreateIntFunction);
    runtime->AddNativeFunctionL(_L("create_str"), 3, 3, CreateStringFunction);
    runtime->AddNativeFunctionL(_L("delete"), 2, 2, DeleteFunction);
    runtime->AddNativeFunctionL(_L("about"), 0, 0, AboutFunction);
  }
  
  Runtime::Value ExecuteL(TInt index, Runtime::Value *params,
                          TInt paramCount, TRequestStatus &status)
 {

    // a variable which will contain the function result
    Runtime::Value result;
    result.SetNull(); // default for functions returning null
    switch (index)
	{
		case GetIntFunction:
		{
			result = GetIntL(params, paramCount);
			break;
		}

		case GetStringFunction:
		{
			result = GetStringL(params, paramCount);
			break;
		}

		case SetIntFunction:
		{
			result = SetIntL(params, paramCount);
			break;
		}

		case SetStringFunction:
		{
			result = SetStringL(params, paramCount);
			break;
		}

		case CreateIntFunction:
		{
			result = CreateIntL(params, paramCount);
			break;
		}

		case CreateStringFunction:
		{
			result = CreateStringL(params, paramCount);
			break;
		}

		case DeleteFunction:
		{
			result = DeleteRepL(params, paramCount);
			break;
		}

		case AboutFunction:
		{
			result = runtime->NewStringL(KAboutString);
			break;
		}
    }
    return result;
  }

};

// the first exported function creates an instance of this module
EXPORT_C NativeModule* NewCenRepModuleL() { return new(ELeave) CenRepModule; }

// the second exported function is required by DLL conventions (2nd ed only)
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason) { return KErrNone; }
#endif
