#include "TestActor.h"
#include "Engine/Engine.h"

// 首先，在包含任何PCL头文件前定义这些宏
#define PCL_NO_PRECOMPILE  // 关键：告诉PCL不要预编译，使用显式实例化
#define BOOST_USE_WINDOWS_H 0
#define NOMINMAX

// 使用UE的第三方宏隔离
THIRD_PARTY_INCLUDES_START

// 先包含Eigen
#include <Eigen/Dense>
#include <Eigen/StdVector>

// 再包含PCL
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

THIRD_PARTY_INCLUDES_END

// 构造函数
ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// BeginPlay
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 添加初始测试输出
	FString InitMessage = TEXT("TestActor已开始运行，准备进行测试...");
	
	// 检查GEngine是否可用
	if (GEngine)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEngine可用，尝试输出到屏幕"));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, InitMessage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine不可用，无法输出到屏幕"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("控制台日志：%s"), *InitMessage);
	
	// 调试输出：准备调用TestPCL
	UE_LOG(LogTemp, Warning, TEXT("准备调用TestPCL函数..."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("准备调用TestPCL函数..."));
	}
	
	// 测试PCL和Eigen
	TestPCL();
	TestEigen();
}

// Tick
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 每帧在控制台和屏幕上输出信息
	static float totalTime = 0.0f;
	totalTime += DeltaTime;
	
	// 每秒输出一次
	static float lastOutputTime = 0.0f;
	if (totalTime - lastOutputTime > 1.0f)
	{
		lastOutputTime = totalTime;
		UE_LOG(LogTemp, Warning, TEXT("Tick函数正在运行，总时间：%.2f秒"), totalTime);
		
		if (GEngine)
		{
			FString tickMessage = FString::Printf(TEXT("Tick: %.2f秒"), totalTime);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, tickMessage);
		}
	}
}

// PCL测试函数 - 使用完全不同的方法定义点
void ATestActor::TestPCL()
{
	// 进入函数调试输出
	UE_LOG(LogTemp, Warning, TEXT("进入TestPCL函数..."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("进入TestPCL函数..."));
	}
	
	try 
	{
		// 尝试创建点调试输出
		UE_LOG(LogTemp, Warning, TEXT("准备创建点数组..."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("准备创建点数组..."));
		}
	
		// 创建一个简单的自定义点结构，而不使用PCL的PointXYZ
		struct MyPoint {
			float x;
			float y;
			float z;
		};
		
		// 创建一个点数组
		TArray<MyPoint> points;
		points.SetNum(5);
		
		// 填充成功调试输出
		UE_LOG(LogTemp, Warning, TEXT("点数组已创建..."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("点数组已创建..."));
		}
		
		// 填充点数据
		for (int32 i = 0; i < points.Num(); ++i)
		{
			points[i].x = 1.0f * i;
			points[i].y = 2.0f * i;
			points[i].z = 3.0f * i;
		}
		
		// 添加窗口输出
		FString Message = FString::Printf(TEXT("PCL测试替代方案: 创建了包含 %d 个点的数组"), points.Num());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
		}
	}
	catch (const std::exception& e)
	{
		FString ErrorMessage = FString::Printf(TEXT("PCL测试失败: %s"), UTF8_TO_TCHAR(e.what()));
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
		}
	}
	catch (...)
	{
		// 捕获任何异常
		UE_LOG(LogTemp, Error, TEXT("PCL测试失败: 未知异常"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PCL测试失败: 未知异常"));
		}
	}
	
	// 函数结束调试输出
	UE_LOG(LogTemp, Warning, TEXT("TestPCL函数执行完毕"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("TestPCL函数执行完毕"));
	}
}

// Eigen测试函数
void ATestActor::TestEigen()
{
	try 
	{
		// 使用Eigen命名空间，但不经过PCLTypes命名空间
		Eigen::Matrix3f rotation;
		rotation = Eigen::AngleAxisf(1.0f, Eigen::Vector3f::UnitZ());
		
		// 打印矩阵的一些值
		float angle = atan2(rotation(1, 0), rotation(0, 0));
		
		// 添加窗口输出
		FString Message = FString::Printf(TEXT("Eigen测试成功: 创建了旋转矩阵，角度约为 %.2f 弧度"), angle);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
		}
	}
	catch (const std::exception& e)
	{
		FString ErrorMessage = FString::Printf(TEXT("Eigen测试失败: %s"), UTF8_TO_TCHAR(e.what()));
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
		}
	}
	catch (...)
	{
		// 捕获任何异常
		UE_LOG(LogTemp, Error, TEXT("Eigen测试失败: 未知异常"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Eigen测试失败: 未知异常"));
		}
	}
}