// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleHttpPrivatePCH.h"
#include "SimpleHttpInstance.h"


DEFINE_LOG_CATEGORY(SimpleHttp)


// Sets default values
ASimpleHttpInstance::ASimpleHttpInstance()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASimpleHttpInstance::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASimpleHttpInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASimpleHttpInstance::StartHttpRequest(const FString& url, const EHttpRequestType requestType, const TArray<FHttpHeaderValue>& listHeaderNameValue, const FString& content)
{
	if (url.IsEmpty())
	{
		UE_LOG(SimpleHttp, Error, TEXT("URL is empty."));
		return;
	}


	const UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHttpRequestType"), true);
	FString verb;
	if (!pEnum)
	{
		verb = "GET";
	}
	else
	{
		verb = pEnum->GetEnumName((int32)requestType);
	}

	if (Http == nullptr)
	{
		Http = &FHttpModule::Get();
	}
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ASimpleHttpInstance::OnResponseReceivedInternal);

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb(verb);
	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	//Request->SetHeader("Content-Type", TEXT("application/json"));
	for (int i = 0; i < listHeaderNameValue.Num(); i++)
	{
		FHttpHeaderValue httpHeader = listHeaderNameValue[i];
		if (httpHeader.Name.IsEmpty() || httpHeader.Value.IsEmpty())
		{
			continue;
		}
		Request->SetHeader(httpHeader.Name, httpHeader.Value);
	}
	Request->SetContentAsString(content);

	Request->ProcessRequest();
}


void ASimpleHttpInstance::OnResponseReceivedInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString contentStr = Response->GetContentAsString();
	UE_LOG(SimpleHttp, Log, TEXT("OnResponseReceivedInternal: contentStr=%s"), *contentStr);

	this->OnResponseReceived(contentStr, bWasSuccessful);
}

void ASimpleHttpInstance::ParseJsonAsString(const FString& contentString, const FString& jsonKey, FString& result)
{
	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(contentString);

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> jsonObject;

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(reader, jsonObject))
	{
		TMap<FString, TSharedPtr<FJsonValue>> mapValue = jsonObject->Values;
		TSharedPtr<FJsonValue> value = mapValue[jsonKey];
		if (value.IsValid())
		{
			result = value->AsString();
		}
	}
	else
	{
		UE_LOG(SimpleHttp, Error, TEXT("ParseJson: JSON syntax error"));
	}
}

