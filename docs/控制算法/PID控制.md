---
title: PID 闭环控制
---

# PID 闭环控制

## 误差与反馈

闭环系统测量实际输出，并根据目标值与测量值的差修正执行器输入。定义误差 `e(t)=r(t)-y(t)`，连续 PID 为：

```text
u(t) = Kp·e(t) + Ki·∫e(t)dt + Kd·de(t)/dt
```

比例项响应当前误差；积分项累积误差，在闭环稳定且执行器有足够能力等条件下消除恒定偏差；微分项响应变化率，能够增加阻尼，但会放大高频测量噪声。增大增益不必然改善性能，过大增益可能引起振荡或失稳。

## 离散位置式与增量式

固定采样周期为 `T`，采用矩形积分与后向差分时：

```text
I[k] = I[k-1] + Ki·T·e[k]
D[k] = Kd·(e[k]-e[k-1])/T
u[k] = Kp·e[k] + I[k] + D[k]
```

位置式直接计算全量输出。相减后得到对应的增量式：

```text
Δu[k] = Kp·(e[k]-e[k-1]) + Ki·T·e[k]
      + Kd·(e[k]-2e[k-1]+e[k-2])/T
u[k] = u[k-1] + Δu[k]
```

若实现中已将采样周期合入 Ki、Kd，再次修改周期时必须重新换算系数。输出累加器和误差历史需要明确初始化。

## 积分饱和与输出约束

电机 PWM、电流或转矩都有物理上限。输出饱和时继续累积积分会导致恢复缓慢和超调，可采用积分限幅、条件积分或反算抗饱和。积分分离在误差较大时抑制积分；变速积分则使积分权重随误差变化，避免硬阈值造成的突变。

## 微分改进、死区与补偿

对测量值计算负微分 `-Kd·(y[k]-y[k-1])/T` 可以减轻目标阶跃引起的微分冲击。微分项增加一阶低通滤波可以抑制噪声，但同时引入延迟。

输入死区可减少量化噪声附近的频繁动作，也会允许一定残余误差。输出偏移可用于补偿执行器静摩擦或启动死区，需要限制方向和幅度，防止零误差时持续施力。

## 位置式 PID 实现

以下 C 实现使用测量值微分，积分项与输出分别限幅。`dt` 的单位为秒，增益与时间单位须一致；输入和参数须为有限数，且各下限不大于上限。首次调用不计算微分，避免将初始测量值误认为一个变化量。

```c
#include <stdbool.h>

typedef struct {
    float kp, ki, kd;
    float integral;
    float previousMeasurement;
    float integralMin, integralMax;
    float outputMin, outputMax;
    bool initialized;
} Pid;

static float clampValue(float value, float minimum, float maximum)
{
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

float pidStep(Pid *pid, float target, float measurement, float dt)
{
    if (!(dt > 0.0f)) {
        return clampValue(0.0f, pid->outputMin, pid->outputMax);
    }

    float error = target - measurement;
    float derivative = 0.0f;
    if (pid->initialized) {
        derivative = -pid->kd *
                     (measurement - pid->previousMeasurement) / dt;
    }
    pid->integral = clampValue(pid->integral + pid->ki * error * dt,
                               pid->integralMin, pid->integralMax);
    pid->previousMeasurement = measurement;
    pid->initialized = true;

    return clampValue(pid->kp * error + pid->integral + derivative,
                       pid->outputMin, pid->outputMax);
}
```

控制器可以用指定初始化器建立初始状态，随后由固定周期任务调用。下列数值仅用于说明初始化与调用，不代表某台电机的整定参数；返回值的正负方向须由执行器驱动解释。

```c
Pid speedPid = {
    .kp = 1.0f, .ki = 0.5f, .kd = 0.01f,
    .integralMin = -20.0f, .integralMax = 20.0f,
    .outputMin = -100.0f, .outputMax = 100.0f
};

/* 目标、测量单位为同一速度单位，控制周期为 10 ms。 */
float command = pidStep(&speedPid, 50.0f, 42.0f, 0.01f);
```

积分限幅只约束积分状态，不会自动消除执行器饱和期间的全部积分积累；需要更快退出饱和时，可增加条件积分或反算抗饱和。此实现未加入微分滤波，带噪声测量需在明确带宽后扩展。

## 编码电机与双环控制

编码器在采样窗口内的计数增量可换算为速度；每转计数需同时考虑线数、倍频和减速比。测量方向、控制方向与误差符号必须一致，使闭环保持负反馈。

位置外环产生速度目标，速度内环产生 PWM 或电流目标。内环通常具有更高带宽，外环输出受速度上限约束。倒立摆等系统还需区分起摆和稳态平衡的控制模式；单一线性 PID 不能保证覆盖所有工作状态。
