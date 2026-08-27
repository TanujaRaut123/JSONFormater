#include "DecantingPage.h"

#include "../Components/SectionCard.h"

#include <QLabel>
#include <QVBoxLayout>

DecantingPage::DecantingPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("decantingPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 28, 30, 30);
    auto *card = new SectionCard(QStringLiteral("Decanting"), this);
    card->contentLayout()->addWidget(new QLabel(QStringLiteral("Decanting workspace ready"), card));
    card->contentLayout()->addStretch();
    layout->addWidget(card, 1);
}
