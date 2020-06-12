#include "unitfilter.h"

#include <algorithm>

UnitFilter::UnitFilter(QSharedPointer<QSqlTableModel> model, QWidget *parent)
    :QWidget(parent), model(model)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    checkBox = new QCheckBox(this);
    mainLayout->addWidget(checkBox);

    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    for (int index = 0; index < model->columnCount(); ++index) {
        const QString text = model->headerData(index, Qt::Horizontal, Qt::DisplayRole).toString();

        QLabel *label = new QLabel(text + ":", this);
        editors.push_back(new QLineEdit(this));
        editors.back()->setSizePolicy(sizePolicy);

        mainLayout->addWidget(label);
        mainLayout->addWidget(editors.back());
    }

    QLineEdit* idEdit = dynamic_cast<QLineEdit *>(*editors.begin());
    idEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), idEdit)); // only number allowed to input

    setLayout(mainLayout);
}

bool UnitFilter::isChecked() { return checkBox->isChecked(); }

QString UnitFilter::getFilter()
{
    QString filter;
    bool firstFlag = true; // judge whether to add "and"

    QLineEdit *idEdit = dynamic_cast<QLineEdit *>(editors[0]);

    if (!idEdit->text().isEmpty()) { // handle id first
        filter += model->headerData(0, Qt::Horizontal, -1).toString() + "=" + idEdit->text();
        firstFlag = false;
    }

    for (int index = 1; index < editors.size(); ++index) {
        if (strcmp(editors[index]->metaObject()->className(), "QLineEdit") == 0) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editors[index]);

            if (!lineEdit->text().isEmpty()) {
                filter += (firstFlag ? firstFlag = false, "" : " and ") + model->headerData(index, Qt::Horizontal, -1).toString()
                            + " like \'"  + lineEdit->text() + "\'";
            }
        } else {
            qWarning() << "Illegal pointer in unit filter";
        }
    }

    return firstFlag ? "" : "(" + filter + ")";
}

UnitFilter* UnitFilter::generateNullCopy(QWidget *parent)
{
    return new UnitFilter(model, parent);
}

UnitFilterForRelation::UnitFilterForRelation(QSharedPointer<QSqlRelationalTableModel> model, QWidget *parent)
        :UnitFilter(parent), model(model)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    checkBox = new QCheckBox(this);
    mainLayout->addWidget(checkBox);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    for (int index = 0; index < model->columnCount(); ++index) {
        const QString text = model->headerData(index, Qt::Horizontal, Qt::DisplayRole).toString();
        QLabel *label = new QLabel(text + ":");

        if (model->relation(index).isValid()) { // if is foreign key
            QSqlTableModel* const &relationModel = model->relationModel(index);

            if (!relationModel) {
                qWarning() << "Column" << index << "in" << model->tableName() <<
                              "is not a foreign key.";
                continue;
            }

            QStringList itemList;
            for (int id = 0; id < relationModel->rowCount(); ++id)
                itemList << relationModel->data(relationModel->index(id, 1)).toString();

            editors.push_back(new QComboBox(this));
            QComboBox* comboBox = dynamic_cast<QComboBox *>(editors.back());
            comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
            comboBox->addItems(itemList);
            comboBox->setCurrentIndex(-1); //reserve negative for select all
            comboBox->setSizePolicy(sizePolicy);

            mainLayout->addWidget(label);
            mainLayout->addWidget(comboBox);
        } else { // if not
            editors.push_back(new QLineEdit(this));
            editors.back()->setSizePolicy(sizePolicy);

            mainLayout->addWidget(label);
            mainLayout->addWidget(editors.back());
        }
    }

    setLayout(mainLayout);
}

QString UnitFilterForRelation::getFilter()
{
    QString filter;
    bool firstFlag = true; // judge whether to add "and"

    QLineEdit *idEdit = dynamic_cast<QLineEdit *>(editors[0]);

    if (!idEdit->text().isEmpty()) { // handle id first
        filter += model->headerData(0, Qt::Horizontal, -1).toString() + "=" + idEdit->text();
        firstFlag = false;
    }

    for (int index = 1; index < editors.size(); ++index) {
        if (strcmp(editors[index]->metaObject()->className(), "QLineEdit") == 0) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editors[index]);

            if (!lineEdit->text().isEmpty()) {
                filter += (firstFlag ? firstFlag = false, "" : " and ") + model->headerData(index, Qt::Horizontal, -1).toString()
                            + " like \'"  + lineEdit->text() + "\'";
            }
        } else if (strcmp(editors[index]->metaObject()->className(), "QComboBox") == 0) {
            QComboBox *comboBox = dynamic_cast<QComboBox *>(editors[index]);

            if (comboBox->currentIndex() >= 0) {
                filter += (firstFlag ? firstFlag = false, "" : " and ") + model->headerData(index, Qt::Horizontal, -1).toString()
                            + "=" + QVariant(comboBox->currentIndex()).toString();
            }
        } else {
            qWarning() << "Illegal pointer in unit filter";
        }
    }

    return firstFlag ? "" : "(" + filter + ")";
}

UnitFilter* UnitFilterForRelation::generateNullCopy(QWidget *parent)
{
    return new UnitFilterForRelation(model, parent);
}
