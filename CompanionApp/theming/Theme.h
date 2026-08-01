#pragma once

#include <QObject>
#include <QString>
#include <QDebug>

#define PROP_DECLARATION(name) Q_PROPERTY(QString name READ name NOTIFY name ## Changed)

#define PROP_ACCESSOR(name) \
    public: \
    QString name() { return m_ ## name; } \
    void set_ ## name(const QString& color) { m_ ## name = color; emit name ## Changed(color); }

// Had to use 'Q_SIGNALS:' rather than 'signals:'
// It seems using the 'signals' macro directly causes issues during pre-processor step
#define PROP_NOTIFY(name) Q_SIGNALS: void name ## Changed(const QString& color)

#define PROP_MEMBER(name, value) private: QString m_ ## name{ value }

#define COLOR_PROPERTY(name, value) \
    PROP_DECLARATION(name)\
    PROP_ACCESSOR(name)\
    PROP_NOTIFY(name);\
    PROP_MEMBER(name, value);


namespace theme {
    enum class Type {
        Dark = 0,
        Light
    };

    class Theme : public QObject {
        Q_OBJECT

        COLOR_PROPERTY(textPrimary, "#e1ecd6");
        COLOR_PROPERTY(textSecondary, "#afafaf");
        COLOR_PROPERTY(textDisabled, "#919191");

        COLOR_PROPERTY(buttonPrimaryNormal, "#3f4d36");
        COLOR_PROPERTY(buttonPrimaryHovered, "#495242");
        COLOR_PROPERTY(buttonPrimaryPressed, "#3c4536");
        COLOR_PROPERTY(buttonPrimaryDisabled, "#31372f");

        COLOR_PROPERTY(buttonSecondaryNormal, "transparent");
        COLOR_PROPERTY(buttonSecondaryHovered, "#474747");
        COLOR_PROPERTY(buttonSecondaryPressed, "#393939");
        COLOR_PROPERTY(buttonSecondaryDisabled, "transparent");

        COLOR_PROPERTY(backgroundPrimary, "#1e1e1e");
        COLOR_PROPERTY(backgroundSecondary, "#252525");
        COLOR_PROPERTY(backgroundTertiary, "#2e2e2e");
        COLOR_PROPERTY(backgroundBackdrop, "#60000000");

        COLOR_PROPERTY(accentPrimaryNormal, "#414e39");
        COLOR_PROPERTY(accentPrimaryHovered, "#526049");
        COLOR_PROPERTY(error, "#b21a1a");
        COLOR_PROPERTY(border, "#646464");

        Type mType{ Type::Dark };

    public:
        explicit Theme(Type type, QObject* parent = nullptr)
            : mType(type)
            , QObject(parent) {
            qDebug() << "Theme::Theme";
        }

        ~Theme() {
            qDebug() << "Theme::~Theme";
        }

        Q_INVOKABLE bool isDark() const { return mType == Type::Dark; }

        Type getType() const { return mType; }

    };
}
