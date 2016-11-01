// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include "SimpleHttpInstance.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(SimpleHttp, Log, All);


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EHttpRequestType
{
	GET			UMETA(DisplayName = "GET"),
	POST		UMETA(DisplayName = "POST")
};

USTRUCT(BlueprintType)
struct FHttpHeaderValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value;
};


UCLASS()
class SIMPLEHTTP_API ASimpleHttpInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleHttpInstance();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SimpleHttp")
	void StartHttpRequest(const FString& url, const EHttpRequestType requestType, const TArray<FHttpHeaderValue>& listHeaderNameValue, const FString& content);

	UFUNCTION(BlueprintImplementableEvent, Category = "SimpleHttp")
	void OnResponseReceived(const FString& responseString, const bool isSuccess);


	UFUNCTION(BlueprintCallable, Category = "SimpleHttp")
	void ParseJsonAsString(const FString& contentString, const FString& jsonKey, FString& result);


private:

	FHttpModule* Http;

	/* Assign this function to call when the GET request processes sucessfully */
	void OnResponseReceivedInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


};
