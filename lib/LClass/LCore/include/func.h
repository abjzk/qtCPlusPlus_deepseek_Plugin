﻿#pragma once
#include <QString>
#include <QSettings>
#include "lcore_global.h"
#include <QIcon>
#include <QColor>
#include <QPainter>
namespace jzk
{
	class LCORE_EXPORT LFunc
	{
	public:
		// 将字符串中的特殊字符转义
		static QString escapeSpecialCharacters(const QString& text);
		// 将字符串中的tab和空格替换为html的标签
		static QString replaceSpaceToHtml(QString& text);
		// 将字符串中的特殊字符转义或反转义
		static QString escapeString(QString text, bool escape);
#ifdef WIN32
		// windows10 以上读取系统是否处于深色模式
		static bool IsSystemDarkModeActive();
		static QColor getSystemAccentColor();
#endif
		/**
		 * 开机自启动，windows系统采用注册表方式，linux系统采用配置文件方式
		 * @param isAutoRun 是否开机自启动
		 * @param appName 应用名
		 */
		static void autoRun(int isAutoRun, QString appName);

		static void autoRunPath(int isAutoRun, QString appName ,QString appPath ,QString params);

		/**
		 * @brief 截断字符串并添加省略号
		 * @param str 需要截断的字符串
		 * @param length 截断长度
		 * @return
		 */
		static QString truncateString(const QString& str, int length);

		template<typename T, typename... Args>
		static QString FString(T&& arg, Args&&... args)
		{
			QString format = "";
			int size = sizeof...(args) + 1; // 计算参数包的大小
			for (int i = 1; i <= size; ++i) // 循环添加参数格式化标识符
			{
				format.append("%");
				format.append(QString::number(i));
			}

			// 将所有参数添加到 format 中
			QString result = format.arg(std::forward<T>(arg));
			using expander = int[];
			(void)expander {
				0, (void(result = result.arg(std::forward<Args>(args))), 0)...
			};
			return result;
		}
		static QIcon setIconColor(const QIcon& icon, const QColor& color);

		static QColor oppositeColor(const QColor& color);
		static QIcon getoppositeColorIcon(const QIcon& icon, const QColor& color)
		{
			return setIconColor(icon, oppositeColor(color));
		}
		static QDateTime chrono_time_point_to_QDateTime(const std::chrono::system_clock::time_point& time);
	};
}
