#pragma once

// 先包含UE核心头文件
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// 不再前向声明PCL类型，而是完全从头文件中移除PCL依赖
// 所有PCL相关代码都放在cpp文件中实现

#include "TestActor.generated.h"

UCLASS()
class DRONE_API ATestActor : public AActor
{
	GENERATED_BODY()
       
public:	
	ATestActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// 测试PCL点云类型 - 实现在cpp文件中，不在头文件中引用PCL
	void TestPCL();
       
	// 测试Eigen矩阵 - 实现在cpp文件中，不在头文件中引用Eigen
	void TestEigen();
};