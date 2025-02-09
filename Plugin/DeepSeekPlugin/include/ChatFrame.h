#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

enum class Role
{
    User,
    Assistant
};
static QString roleToString(Role role)
{
    switch (role)
    {
    case Role::User:
        return "user";
    case Role::Assistant:
        return "assistant";
    default:
        return "";
    }
}
static Role stringToRole(QString role)
{
    if (role == "user")
    {
        return Role::User;
    }
    else if (role == "assistant")
    {
        return Role::Assistant;
    }
    return Role::User;
}

class ChatFrame : public QFrame
{
    Q_OBJECT
public:
    ChatFrame(Role role, QWidget *parent = nullptr);
    ~ChatFrame();

private:
    void initUi();
    void initConnect();

private:
    Role _role;
    QLabel *_roleLabel = new QLabel(this);
    QWidget *_oneRowWidget = new QWidget(this);
};