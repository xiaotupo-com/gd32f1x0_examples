# FreeRTOS 滴答定时器钩子函数 & 按键消抖例子

## 1. 使能滴答定时器钩子函数

要使用滴答定时器钩子函数，需要去 `FreeRTOSConfig.h` 文件中使能：
```c
#define configUSE_TICK_HOOK 1
```

然后实现 `void vApplicationTickHook(void)` 函数：

```c
void vApplicationTickHook(void)
{
    //	/* 1sm 执行一次 */
	FlagStatus key_state = USER_KEY_STATE; /* 读取按键当前状态 */

	user_key.key_buf = (user_key.key_buf << 1) | key_state; /* key_buf 左移一位，并或上按键状态 key_state */

	/* 如果 32sm 内 32次读到的状态都为 0 代表按键按下了 */
	if ((user_key.key_buf == (uint32_t)0x00000000) && (user_key.ok_down_flag == 0))
	{
		user_key.ok_down_flag = 1;
	}

	/* 如果 ok_down_flag 等于 1  */
	if (user_key.ok_down_flag == 1)
	{
		user_key.long_down_ms++;
	}

	/* 如果 long_down_ms 小于 LONG_DOWN_FLAG_MS 则为短按 */
	if ((user_key.key_buf == (uint32_t)0xffffffff) && (user_key.long_down_ms < LONG_DOWN_FLAG_MS) && (user_key.ok_down_flag == 1))
	{
		user_key.state = CLICK; /* 单击 */
	}

	if (user_key.long_down_ms > LONG_DOWN_FLAG_MS && user_key.key_buf == (uint32_t)0x00000000)
	{
		user_key.state = HOLD_OR_LOND_DOWN;
	}
}
```