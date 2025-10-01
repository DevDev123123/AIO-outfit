#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStandardPaths>
#include <QDir>
#include <QProgressDialog>
#include <QTextStream>
#include <QRegularExpression>
#include <QTabWidget>
#include <QListWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QComboBox>
#include <QTime>

// Format enumeration
enum class OutfitFormat {
    Unknown,
    Cherax,
    YimMenu,
    Lexis,
    Stand
};

// ManualFormatSelector class definition (integrated from format_selector.h)
class ManualFormatSelector : public QWidget {
    Q_OBJECT
public:
    explicit ManualFormatSelector(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
    }
    
    QString getSourceFormat() const {
        return sourceFormatCombo->currentText();
    }
    
    QString getTargetFormat() const {
        return targetFormatCombo->currentText();
    }
    
    bool isManualMode() const {
        return manualModeRadio->isChecked();
    }
    
signals:
    void modeChanged(bool isManual);
    void formatChanged();
    
private:
    void setupUI() {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        
        QGroupBox* modeBox = new QGroupBox("Conversion Selection", this);
        modeBox->setStyleSheet(
            "QGroupBox { "
            "  background: #2a2a2a; "
            "  border: 2px solid #444; "
            "  border-radius: 12px; "
            "  margin-top: 10px; "
            "  padding-top: 20px; "
            "  color: #fff; "
            "  font-size: 14px; "
            "  font-weight: bold; "
            "}"
            "QGroupBox::title { "
            "  subcontrol-origin: margin; "
            "  left: 15px; "
            "  padding: 0 5px; "
            "}"
            "QRadioButton { "
            "  color: #fff; "
            "  font-size: 13px; "
            "  font-weight: normal; "
            "  spacing: 8px; "
            "}"
            "QRadioButton::indicator { "
            "  width: 18px; "
            "  height: 18px; "
            "}"
            "QRadioButton::indicator::unchecked { "
            "  border: 2px solid #666; "
            "  border-radius: 9px; "
            "  background: #1a1a1a; "
            "}"
            "QRadioButton::indicator::checked { "
            "  border: 2px solid #667eea; "
            "  border-radius: 9px; "
            "  background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #667eea, stop:1 #667eea); "
            "}"
        );
        
        QVBoxLayout* modeLayout = new QVBoxLayout(modeBox);
        
        autoModeRadio = new QRadioButton("Auto Mode - Automatically detect format", this);
        manualModeRadio = new QRadioButton("Manual Mode - Choose source and target formats", this);
        
        autoModeRadio->setChecked(true);
        
        buttonGroup = new QButtonGroup(this);
        buttonGroup->addButton(autoModeRadio);
        buttonGroup->addButton(manualModeRadio);
        
        connect(manualModeRadio, &QRadioButton::toggled, this, [this](bool checked) {
            formatSelectionWidget->setVisible(checked);
            emit modeChanged(checked);
        });
        
        modeLayout->addWidget(autoModeRadio);
        modeLayout->addWidget(manualModeRadio);
        
        mainLayout->addWidget(modeBox);
        
        // Manual format selection widget
        formatSelectionWidget = new QWidget(this);
        formatSelectionWidget->setVisible(false);
        
        QVBoxLayout* formatLayout = new QVBoxLayout(formatSelectionWidget);
        formatLayout->setContentsMargins(0, 10, 0, 0);
        
        QGroupBox* formatBox = new QGroupBox("Format Selection", this);
        formatBox->setStyleSheet(
            "QGroupBox { "
            "  background: #2a2a2a; "
            "  border: 2px solid #444; "
            "  border-radius: 12px; "
            "  margin-top: 10px; "
            "  padding-top: 20px; "
            "  color: #fff; "
            "  font-size: 14px; "
            "  font-weight: bold; "
            "}"
            "QGroupBox::title { "
            "  subcontrol-origin: margin; "
            "  left: 15px; "
            "  padding: 0 5px; "
            "}"
            "QLabel { "
            "  color: #fff; "
            "  font-size: 13px; "
            "  font-weight: bold; "
            "}"
            "QComboBox { "
            "  background: #1a1a1a; "
            "  color: #fff; "
            "  border: 2px solid #555; "
            "  padding: 8px; "
            "  border-radius: 6px; "
            "  font-size: 13px; "
            "}"
            "QComboBox::drop-down { "
            "  border: none; "
            "  width: 20px; "
            "}"
            "QComboBox::down-arrow { "
            "  image: none; "
            "  border-left: 5px solid transparent; "
            "  border-right: 5px solid transparent; "
            "  border-top: 5px solid #fff; "
            "  margin-right: 5px; "
            "}"
            "QComboBox QAbstractItemView { "
            "  background: #2a2a2a; "
            "  color: #fff; "
            "  selection-background-color: #667eea; "
            "  border: 2px solid #555; "
            "}"
        );
        
        QVBoxLayout* formatBoxLayout = new QVBoxLayout(formatBox);
        
        // Source format
        QHBoxLayout* sourceLayout = new QHBoxLayout();
        QLabel* sourceLabel = new QLabel("Source Format:", this);
        sourceFormatCombo = new QComboBox(this);
        sourceFormatCombo->addItems({"Cherax", "YimMenu", "Lexis", "Stand"});
        connect(sourceFormatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                this, &ManualFormatSelector::formatChanged);
        
        sourceLayout->addWidget(sourceLabel);
        sourceLayout->addWidget(sourceFormatCombo, 1);
        formatBoxLayout->addLayout(sourceLayout);
        
        // Arrow indicator
        QLabel* arrowLabel = new QLabel("⬇", this);
        arrowLabel->setAlignment(Qt::AlignCenter);
        arrowLabel->setStyleSheet("color: #667eea; font-size: 24px; margin: 5px 0;");
        formatBoxLayout->addWidget(arrowLabel);
        
        // Target format
        QHBoxLayout* targetLayout = new QHBoxLayout();
        QLabel* targetLabel = new QLabel("Target Format:", this);
        targetFormatCombo = new QComboBox(this);
        targetFormatCombo->addItems({"YimMenu", "Cherax", "Lexis", "Stand"});
        connect(targetFormatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                this, &ManualFormatSelector::formatChanged);
        
        targetLayout->addWidget(targetLabel);
        targetLayout->addWidget(targetFormatCombo, 1);
        formatBoxLayout->addLayout(targetLayout);
        
        // Info label
        QLabel* infoLabel = new QLabel(
            "💡 Tip: Select the format of your input file and the desired output format", 
            this
        );
        infoLabel->setWordWrap(true);
        infoLabel->setStyleSheet("color: #888; font-size: 11px; font-weight: normal; margin-top: 10px;");
        formatBoxLayout->addWidget(infoLabel);
        
        formatLayout->addWidget(formatBox);
        mainLayout->addWidget(formatSelectionWidget);
    }
    
private:
    QRadioButton* autoModeRadio;
    QRadioButton* manualModeRadio;
    QButtonGroup* buttonGroup;
    QWidget* formatSelectionWidget;
    QComboBox* sourceFormatCombo;
    QComboBox* targetFormatCombo;
};

// Conversion Functions
QJsonObject cheraxToYim(const QJsonObject& cherax) {
    QJsonObject yim;
    
    if (cherax.contains("model")) {
        yim["model"] = cherax["model"].toInteger();
    }
    
    QJsonObject blendData;
    blendData["is_parent"] = 0;
    blendData["shape_first_id"] = 0;
    blendData["shape_mix"] = 0.0;
    blendData["shape_second_id"] = 0;
    blendData["shape_third_id"] = 0;
    blendData["skin_first_id"] = 0;
    blendData["skin_mix"] = 0.0;
    blendData["skin_second_id"] = 0;
    blendData["skin_third_id"] = 0;
    blendData["third_mix"] = 0.0;
    yim["blend_data"] = blendData;
    
    QMap<QString, int> componentMap = {
        {"Head", 0}, {"Beard", 1}, {"Hair", 2}, {"Torso", 3},
        {"Legs", 4}, {"Hands", 5}, {"Feet", 6}, {"Teeth", 7},
        {"Special", 8}, {"Special 2", 9}, {"Decal", 10}, {"Tuxedo/Jacket Bib", 11}
    };
    
    QJsonObject components;
    if (cherax.contains("components")) {
        QJsonObject cheraxComps = cherax["components"].toObject();
        for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
            if (cheraxComps.contains(it.key())) {
                QJsonObject comp = cheraxComps[it.key()].toObject();
                QJsonObject yimComp;
                yimComp["drawable_id"] = comp.value("drawable").toInt();
                yimComp["texture_id"] = comp.value("texture").toInt();
                components[QString::number(it.value())] = yimComp;
            }
        }
    }
    yim["components"] = components;
    
    QMap<QString, int> propsMap = {
        {"Head", 0}, {"Eyes", 1}, {"Ears", 2}, {"Mouth", 3},
        {"Left Hand", 4}, {"Right Hand", 5}, {"Left Wrist", 6},
        {"Right Wrist", 7}, {"Hip", 8}
    };
    
    QJsonObject props;
    if (cherax.contains("props")) {
        QJsonObject cheraxProps = cherax["props"].toObject();
        for (auto it = propsMap.begin(); it != propsMap.end(); ++it) {
            if (cheraxProps.contains(it.key())) {
                QJsonObject prop = cheraxProps[it.key()].toObject();
                QJsonObject yimProp;
                yimProp["drawable_id"] = prop.value("drawable").toInt();
                yimProp["texture_id"] = prop.value("texture").toInt();
                props[QString::number(it.value())] = yimProp;
            }
        }
    }
    yim["props"] = props;
    
    return yim;
}

QJsonObject yimToCherax(const QJsonObject& yim) {
    QJsonObject cherax;
    
    cherax["format"] = "Cherax Entity";
    cherax["type"] = 2;
    
    if (yim.contains("model")) {
        cherax["model"] = yim["model"].toInteger();
    }
    
    cherax["baseFlags"] = 66855;
    
    QMap<int, QString> componentMap = {
        {0, "Head"}, {1, "Beard"}, {2, "Hair"}, {3, "Torso"},
        {4, "Legs"}, {5, "Hands"}, {6, "Feet"}, {7, "Teeth"},
        {8, "Special"}, {9, "Special 2"}, {10, "Decal"}, {11, "Tuxedo/Jacket Bib"}
    };
    
    QJsonObject components;
    if (yim.contains("components")) {
        QJsonObject yimComps = yim["components"].toObject();
        for (auto it = yimComps.begin(); it != yimComps.end(); ++it) {
            int id = it.key().toInt();
            if (componentMap.contains(id)) {
                QJsonObject comp = it.value().toObject();
                QJsonObject cheraxComp;
                cheraxComp["drawable"] = comp.value("drawable_id").toInt();
                cheraxComp["texture"] = comp.value("texture_id").toInt();
                cheraxComp["palette"] = 0;
                components[componentMap[id]] = cheraxComp;
            }
        }
    }
    cherax["components"] = components;
    
    QMap<int, QString> propsMap = {
        {0, "Head"}, {1, "Eyes"}, {2, "Ears"}, {3, "Mouth"},
        {4, "Left Hand"}, {5, "Right Hand"}, {6, "Left Wrist"},
        {7, "Right Wrist"}, {8, "Hip"}
    };
    
    QJsonObject props;
    if (yim.contains("props")) {
        QJsonObject yimProps = yim["props"].toObject();
        for (auto it = yimProps.begin(); it != yimProps.end(); ++it) {
            int id = it.key().toInt();
            if (propsMap.contains(id)) {
                QJsonObject prop = it.value().toObject();
                QJsonObject cheraxProp;
                cheraxProp["drawable"] = prop.value("drawable_id").toInt();
                cheraxProp["texture"] = prop.value("texture_id").toInt();
                props[propsMap[id]] = cheraxProp;
            }
        }
    }
    cherax["props"] = props;
    
    QJsonObject faceFeatures;
    QStringList features = {"Nose Width", "Nose Peak", "Nose Length", "Nose Bone Curveness",
                           "Nose Tip", "Nose Bone Twist", "Eyebrow Height", "Eyebrow Indent",
                           "Cheek Bones", "Cheek Sideways Bone Size", "Cheek Bones Width",
                           "Eye Opening", "Lip Thickness", "Jaw Bone Width", "Jaw Bone Shape",
                           "Chin Bone", "Chin Bone Length", "Chin Bone Shape", "Chin Hole",
                           "Neck Thickness"};
    for (const QString& feature : features) {
        faceFeatures[feature] = 0.0;
    }
    cherax["face_features"] = faceFeatures;
    
    cherax["primary_hair_tint"] = 255;
    cherax["secondary_hair_tint"] = 255;
    cherax["attachments"] = QJsonArray();
    
    return cherax;
}

QJsonObject yimToLexis(const QJsonObject& yim) {
    QJsonObject lexis;
    QJsonObject outfit;
    
    if (yim.contains("model")) {
        outfit["model"] = yim["model"].toInteger();
    }
    
    QJsonArray componentArray;
    QJsonArray componentVarArray;
    
    if (yim.contains("components")) {
        QJsonObject comps = yim["components"].toObject();
        for (int i = 0; i < 12; ++i) {
            QString key = QString::number(i);
            if (comps.contains(key)) {
                QJsonObject comp = comps[key].toObject();
                componentArray.append(comp.value("drawable_id").toInt());
                componentVarArray.append(comp.value("texture_id").toInt());
            } else {
                componentArray.append(0);
                componentVarArray.append(0);
            }
        }
    }
    
    outfit["component"] = componentArray;
    outfit["component variation"] = componentVarArray;
    
    QJsonArray propArray;
    QJsonArray propVarArray;
    
    if (yim.contains("props")) {
        QJsonObject props = yim["props"].toObject();
        for (int i = 0; i < 9; ++i) {
            QString key = QString::number(i);
            if (props.contains(key)) {
                QJsonObject prop = props[key].toObject();
                propArray.append(prop.value("drawable_id").toInt());
                propVarArray.append(prop.value("texture_id").toInt());
            } else {
                propArray.append(-1);
                propVarArray.append(-1);
            }
        }
    }
    
    outfit["prop"] = propArray;
    outfit["prop variation"] = propVarArray;
    
    lexis["outfit"] = outfit;
    return lexis;
}

QString yimToStand(const QJsonObject& yim) {
    QString standText;
    QTextStream stream(&standText);
    
    qint64 model = yim.value("model").toInteger();
    stream << "Model: " << (model == 1885233650 ? "Online Male" : "Online Female") << "\n";
    
    QMap<int, QString> componentNames = {
        {0, "Head"}, {1, "Mask"}, {2, "Hair"}, {3, "Top"},
        {4, "Pants"}, {5, "Gloves / Torso"}, {6, "Shoes"},
        {7, "Accessories"}, {8, "Top 2"}, {9, "Top 3"},
        {10, "Decals"}, {11, "Parachute / Bag"}
    };
    
    if (yim.contains("components")) {
        QJsonObject comps = yim["components"].toObject();
        for (int i = 0; i < 12; ++i) {
            QString key = QString::number(i);
            if (comps.contains(key)) {
                QJsonObject comp = comps[key].toObject();
                stream << componentNames[i] << ": " << comp.value("drawable_id").toInt() << "\n";
                stream << componentNames[i] << " Variation: " << comp.value("texture_id").toInt() << "\n";
            }
        }
    }
    
    QMap<int, QString> propNames = {
        {0, "Hat"}, {1, "Glasses"}, {2, "Earwear"}, 
        {6, "Watch"}, {7, "Bracelet"}
    };
    
    if (yim.contains("props")) {
        QJsonObject props = yim["props"].toObject();
        for (auto it = propNames.begin(); it != propNames.end(); ++it) {
            QString key = QString::number(it.key());
            if (props.contains(key)) {
                QJsonObject prop = props[key].toObject();
                stream << it.value() << ": " << prop.value("drawable_id").toInt() << "\n";
                stream << it.value() << " Variation: " << prop.value("texture_id").toInt() << "\n";
            }
        }
    }
    
    return standText;
}

QJsonObject lexisToYim(const QJsonObject& lexis) {
    QJsonObject yim;
    
    if (lexis.contains("outfit")) {
        QJsonObject outfit = lexis["outfit"].toObject();
        
        if (outfit.contains("model")) {
            yim["model"] = outfit["model"].toInteger();
        }
        
        QJsonObject blendData;
        blendData["is_parent"] = 0;
        blendData["shape_first_id"] = 0;
        blendData["shape_mix"] = 0.0;
        blendData["shape_second_id"] = 0;
        blendData["shape_third_id"] = 0;
        blendData["skin_first_id"] = 0;
        blendData["skin_mix"] = 0.0;
        blendData["skin_second_id"] = 0;
        blendData["skin_third_id"] = 0;
        blendData["third_mix"] = 0.0;
        yim["blend_data"] = blendData;
        
        QJsonObject components;
        if (outfit.contains("component")) {
            QJsonArray compArray = outfit["component"].toArray();
            QJsonArray varArray = outfit["component variation"].toArray();
            
            for (int i = 0; i < compArray.size() && i < 12; ++i) {
                QJsonObject comp;
                comp["drawable_id"] = compArray[i].toInt();
                comp["texture_id"] = (i < varArray.size()) ? varArray[i].toInt() : 0;
                components[QString::number(i)] = comp;
            }
        }
        yim["components"] = components;
        
        QJsonObject props;
        if (outfit.contains("prop")) {
            QJsonArray propArray = outfit["prop"].toArray();
            QJsonArray propVarArray = outfit["prop variation"].toArray();
            
            for (int i = 0; i < propArray.size() && i < 9; ++i) {
                QJsonObject prop;
                prop["drawable_id"] = propArray[i].toInt();
                prop["texture_id"] = (i < propVarArray.size()) ? propVarArray[i].toInt() : -1;
                props[QString::number(i)] = prop;
            }
        }
        yim["props"] = props;
    }
    
    return yim;
}

QString standToYim(const QString& standText) {
    QJsonObject yim;
    QJsonObject blendData;
    blendData["is_parent"] = 0;
    blendData["shape_first_id"] = 0;
    blendData["shape_mix"] = 0.0;
    blendData["shape_second_id"] = 0;
    blendData["shape_third_id"] = 0;
    blendData["skin_first_id"] = 0;
    blendData["skin_mix"] = 0.0;
    blendData["skin_second_id"] = 0;
    blendData["skin_third_id"] = 0;
    blendData["third_mix"] = 0.0;
    yim["blend_data"] = blendData;
    
    QMap<QString, int> standMapping = {
        {"Head:", 0}, {"Mask:", 1}, {"Hair:", 2}, {"Top:", 3},
        {"Pants:", 4}, {"Gloves / Torso:", 5}, {"Shoes:", 6}, 
        {"Accessories:", 7}, {"Top 2:", 8}, {"Top 3:", 9}, 
        {"Decals:", 10}, {"Parachute / Bag:", 11}
    };
    
    QMap<QString, int> standPropsMapping = {
        {"Hat:", 0}, {"Glasses:", 1}, {"Earwear:", 2},
        {"Watch:", 6}, {"Bracelet:", 7}
    };
    
    QJsonObject components;
    QJsonObject props;
    
    QStringList lines = standText.split('\n');
    for (const QString& line : lines) {
        QString trimmed = line.trimmed();
        
        if (trimmed.startsWith("Model:")) {
            QString model = trimmed.mid(6).trimmed();
            yim["model"] = (model.contains("Male") && !model.contains("Female")) ? 1885233650 : -1667301416;
        }
        
        for (auto it = standMapping.begin(); it != standMapping.end(); ++it) {
            if (trimmed.startsWith(it.key())) {
                QString valueStr = trimmed.mid(it.key().length()).trimmed();
                int value = valueStr.toInt();
                
                QString varKey = it.key().replace(":", " Variation:");
                int varValue = 0;
                
                for (const QString& varLine : lines) {
                    if (varLine.trimmed().startsWith(varKey)) {
                        varValue = varLine.mid(varLine.indexOf(':') + 1).trimmed().toInt();
                        break;
                    }
                }
                
                QJsonObject comp;
                comp["drawable_id"] = value;
                comp["texture_id"] = varValue;
                components[QString::number(it.value())] = comp;
                break;
            }
        }
        
        for (auto it = standPropsMapping.begin(); it != standPropsMapping.end(); ++it) {
            if (trimmed.startsWith(it.key())) {
                QString valueStr = trimmed.mid(it.key().length()).trimmed();
                int value = valueStr.toInt();
                
                QString varKey = it.key().replace(":", " Variation:");
                int varValue = -1;
                
                for (const QString& varLine : lines) {
                    if (varLine.trimmed().startsWith(varKey)) {
                        varValue = varLine.mid(varLine.indexOf(':') + 1).trimmed().toInt();
                        break;
                    }
                }
                
                QJsonObject prop;
                prop["drawable_id"] = value;
                prop["texture_id"] = varValue;
                props[QString::number(it.value())] = prop;
                break;
            }
        }
    }
    
    yim["components"] = components;
    yim["props"] = props;
    
    QJsonDocument doc(yim);
    return QString(doc.toJson(QJsonDocument::Indented));
}

OutfitFormat detectFormat(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return OutfitFormat::Unknown;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    if (filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        QString content = QString::fromUtf8(data);
        if (content.contains("Model:") && content.contains("Variation:")) {
            return OutfitFormat::Stand;
        }
        return OutfitFormat::Unknown;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        return OutfitFormat::Unknown;
    }
    
    QJsonObject obj = doc.object();
    
    if (obj.contains("format") && obj["format"].toString() == "Cherax Entity") {
        return OutfitFormat::Cherax;
    }
    
    if (obj.contains("outfit")) {
        QJsonObject outfit = obj["outfit"].toObject();
        if (outfit.contains("component") && outfit.contains("component variation")) {
            return OutfitFormat::Lexis;
        }
    }
    
    if (obj.contains("blend_data") && obj.contains("components")) {
        QJsonObject components = obj["components"].toObject();
        if (!components.isEmpty()) {
            QString firstKey = components.keys().first();
            bool isNumeric = false;
            firstKey.toInt(&isNumeric);
            if (isNumeric) {
                return OutfitFormat::YimMenu;
            }
        }
    }
    
    return OutfitFormat::Unknown;
}

class DropZone : public QWidget {
    Q_OBJECT
public:
    explicit DropZone(QWidget* parent = nullptr) : QWidget(parent), batchMode(false) {
        setAcceptDrops(true);
        setMinimumHeight(200);
        
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);
        
        iconLabel = new QLabel("📁", this);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet("font-size: 48px;");
        
        textLabel = new QLabel("Drop file(s) here or click to browse\nSupports: .json (Cherax/YimMenu/Lexis) and .txt (Stand)", this);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("color: #888; font-size: 13px;");
        
        layout->addWidget(iconLabel);
        layout->addWidget(textLabel);
        
        updateStyle(false);
    }
    
    void setBatchMode(bool batch) {
        batchMode = batch;
        if (batch) {
            iconLabel->setText("📂");
            textLabel->setText("Drop multiple files here or click to browse\nBatch conversion enabled");
        } else {
            iconLabel->setText("📁");
            textLabel->setText("Drop file(s) here or click to browse\nSupports: .json (Cherax/YimMenu/Lexis) and .txt (Stand)");
        }
    }
    
signals:
    void filesDropped(const QStringList& filePaths);
    
protected:
    void dragEnterEvent(QDragEnterEvent* event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();updateStyle(true);
        }
    }
    
    void dragLeaveEvent(QDragLeaveEvent* event) override {
        updateStyle(false);
    }
    
    void dropEvent(QDropEvent* event) override {
        const QMimeData* mimeData = event->mimeData();
        if (mimeData->hasUrls()) {
            QStringList filePaths;
            QList<QUrl> urls = mimeData->urls();
            for (const QUrl& url : urls) {
                QString filePath = url.toLocalFile();
                if (filePath.endsWith(".json", Qt::CaseInsensitive) || 
                    filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                    filePaths.append(filePath);
                }
            }
            if (!filePaths.isEmpty()) {
                emit filesDropped(filePaths);
            }
        }
        updateStyle(false);
    }
    
    void mousePressEvent(QMouseEvent* event) override {
        if (batchMode) {
            QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select Files", "", 
                "Outfit Files (*.json *.txt);;JSON Files (*.json);;Text Files (*.txt)");
            if (!filePaths.isEmpty()) {
                emit filesDropped(filePaths);
            }
        } else {
            QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", 
                "Outfit Files (*.json *.txt);;JSON Files (*.json);;Text Files (*.txt)");
            if (!filePath.isEmpty()) {
                emit filesDropped(QStringList() << filePath);
            }
        }
    }
    
private:
    void updateStyle(bool dragging) {
        if (dragging) {
            setStyleSheet("DropZone { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #667eea, stop:1 #764ba2); border: 3px dashed #fff; border-radius: 16px; }");
        } else {
            setStyleSheet("DropZone { background: #2a2a2a; border: 2px dashed #555; border-radius: 16px; }");
        }
    }
    
    QLabel* iconLabel;
    QLabel* textLabel;
    bool batchMode;
};

// Outfit Editor Tab
class OutfitEditorTab : public QWidget {
    Q_OBJECT
public:
    explicit OutfitEditorTab(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        loadPlayerData();
    }
    
private slots:
    void loadPlayerData() {
        QString yimPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OutfitConverter/YimMenu";
        QDir dir(yimPath);
        
        if (!dir.exists()) {
            playerNameLabel->setText("Player: Unknown");
            return;
        }
        
        playerNameLabel->setText("Player: " + qgetenv("USERNAME"));
        
        outfitList->clear();
        QStringList filters;
        filters << "*.json";
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
        
        for (const QFileInfo& fileInfo : files) {
            outfitList->addItem(fileInfo.baseName());
        }
    }
    
    void onOutfitSelected(QListWidgetItem* item) {
        if (!item) return;
        
        currentOutfitName = item->text();
        QString yimPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OutfitConverter/YimMenu";
        QString filePath = yimPath + "/" + currentOutfitName + ".json";
        
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Failed to load outfit");
            return;
        }
        
        QByteArray data = file.readAll();
        file.close();
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        currentOutfit = doc.object();
        
        outfitNameEdit->setText(currentOutfitName);
        loadOutfitToEditor();
    }
    
    void loadOutfitToEditor() {
        QLayoutItem* item;
        while ((item = componentsLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        componentSpinBoxes.clear();
        textureSpinBoxes.clear();
        
        if (currentOutfit.contains("components")) {
            QJsonObject comps = currentOutfit["components"].toObject();
            
            QStringList compNames = {"Head", "Mask/Beard", "Hair", "Top", "Pants", "Gloves", 
                                     "Shoes", "Accessories", "Undershirt", "Armor", "Decals", "Torso Extra"};
            
            for (int i = 0; i < 12; ++i) {
                QString key = QString::number(i);
                QJsonObject comp = comps.value(key).toObject();
                
                QLabel* label = new QLabel(compNames[i] + ":", this);
                label->setStyleSheet("color: #fff; font-weight: bold;");
                
                QSpinBox* drawableSpin = new QSpinBox(this);
                drawableSpin->setRange(-1, 500);
                drawableSpin->setValue(comp.value("drawable_id").toInt());
                drawableSpin->setStyleSheet("QSpinBox { background: #2a2a2a; color: #fff; border: 1px solid #555; padding: 5px; }");
                
                QSpinBox* textureSpin = new QSpinBox(this);
                textureSpin->setRange(-1, 500);
                textureSpin->setValue(comp.value("texture_id").toInt());
                textureSpin->setStyleSheet("QSpinBox { background: #2a2a2a; color: #fff; border: 1px solid #555; padding: 5px; }");
                
                connect(drawableSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutfitEditorTab::onComponentChanged);
                connect(textureSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutfitEditorTab::onComponentChanged);
                
                componentSpinBoxes[i] = drawableSpin;
                textureSpinBoxes[i] = textureSpin;
                
                QHBoxLayout* rowLayout = new QHBoxLayout();
                rowLayout->addWidget(label, 1);
                rowLayout->addWidget(new QLabel("Drawable:", this));
                rowLayout->addWidget(drawableSpin, 1);
                rowLayout->addWidget(new QLabel("Texture:", this));
                rowLayout->addWidget(textureSpin, 1);
                
                componentsLayout->addLayout(rowLayout);
            }
        }
        
        if (currentOutfit.contains("props")) {
            QJsonObject props = currentOutfit["props"].toObject();
            
            QStringList propNames = {"Hat", "Glasses", "Earwear", "Mouth", "Left Hand", 
                                     "Right Hand", "Watch", "Bracelet", "Hip"};
            
            for (int i = 0; i < 9; ++i) {
                QString key = QString::number(i);
                QJsonObject prop = props.value(key).toObject();
                
                QLabel* label = new QLabel(propNames[i] + ":", this);
                label->setStyleSheet("color: #fff; font-weight: bold;");
                
                QSpinBox* drawableSpin = new QSpinBox(this);
                drawableSpin->setRange(-1, 500);
                drawableSpin->setValue(prop.value("drawable_id").toInt());
                drawableSpin->setStyleSheet("QSpinBox { background: #2a2a2a; color: #fff; border: 1px solid #555; padding: 5px; }");
                
                QSpinBox* textureSpin = new QSpinBox(this);
                textureSpin->setRange(-1, 500);
                textureSpin->setValue(prop.value("texture_id").toInt());
                textureSpin->setStyleSheet("QSpinBox { background: #2a2a2a; color: #fff; border: 1px solid #555; padding: 5px; }");
                
                connect(drawableSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutfitEditorTab::onPropChanged);
                connect(textureSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutfitEditorTab::onPropChanged);
                
                propDrawableSpinBoxes[i] = drawableSpin;
                propTextureSpinBoxes[i] = textureSpin;
                
                QHBoxLayout* rowLayout = new QHBoxLayout();
                rowLayout->addWidget(label, 1);
                rowLayout->addWidget(new QLabel("Drawable:", this));
                rowLayout->addWidget(drawableSpin, 1);
                rowLayout->addWidget(new QLabel("Texture:", this));
                rowLayout->addWidget(textureSpin, 1);
                
                propsLayout->addLayout(rowLayout);
            }
        }
    }
    
    void onComponentChanged() {
        if (currentOutfit.isEmpty()) return;
        
        QJsonObject comps = currentOutfit["components"].toObject();
        
        for (auto it = componentSpinBoxes.begin(); it != componentSpinBoxes.end(); ++it) {
            int idx = it.key();
            QJsonObject comp;
            comp["drawable_id"] = componentSpinBoxes[idx]->value();
            comp["texture_id"] = textureSpinBoxes[idx]->value();
            comps[QString::number(idx)] = comp;
        }
        
        currentOutfit["components"] = comps;
        saveCurrentOutfit();
    }
    
    void onPropChanged() {
        if (currentOutfit.isEmpty()) return;
        
        QJsonObject props = currentOutfit["props"].toObject();
        
        for (auto it = propDrawableSpinBoxes.begin(); it != propDrawableSpinBoxes.end(); ++it) {
            int idx = it.key();
            QJsonObject prop;
            prop["drawable_id"] = propDrawableSpinBoxes[idx]->value();
            prop["texture_id"] = propTextureSpinBoxes[idx]->value();
            props[QString::number(idx)] = prop;
        }
        
        currentOutfit["props"] = props;
        saveCurrentOutfit();
    }
    
    void saveCurrentOutfit() {
        QString yimPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OutfitConverter/YimMenu";
        QString filePath = yimPath + "/" + currentOutfitName + ".json";
        
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            return;
        }
        
        QJsonDocument doc(currentOutfit);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        
        statusLabel->setText("✓ Auto-saved at " + QTime::currentTime().toString("hh:mm:ss"));
        statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px;");
    }
    
    void renameOutfit() {
        if (currentOutfitName.isEmpty()) {
            QMessageBox::warning(this, "Warning", "No outfit selected");
            return;
        }
        
        QString newName = outfitNameEdit->text().trimmed();
        if (newName.isEmpty() || newName == currentOutfitName) {
            return;
        }
        
        QString yimPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OutfitConverter/YimMenu";
        QString oldPath = yimPath + "/" + currentOutfitName + ".json";
        QString newPath = yimPath + "/" + newName + ".json";
        
        if (QFile::exists(newPath)) {
            QMessageBox::warning(this, "Warning", "An outfit with this name already exists");
            return;
        }
        
        if (QFile::rename(oldPath, newPath)) {
            currentOutfitName = newName;
            loadPlayerData();
            statusLabel->setText("✓ Outfit renamed successfully");
            statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px;");
        } else {
            QMessageBox::critical(this, "Error", "Failed to rename outfit");
        }
    }
    
    void exportOutfit() {
        if (currentOutfit.isEmpty()) {
            QMessageBox::warning(this, "Warning", "No outfit selected");
            return;
        }
        
        QString format = exportFormatCombo->currentText();
        QString yimPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/OutfitConverter";
        QString outputPath;
        QString content;
        
        if (format == "Cherax") {
            QJsonObject cherax = yimToCherax(currentOutfit);
            QJsonDocument doc(cherax);
            content = doc.toJson(QJsonDocument::Indented);
            outputPath = yimPath + "/Cherax/" + currentOutfitName + "_exported.json";
        } else if (format == "Lexis") {
            QJsonObject lexis = yimToLexis(currentOutfit);
            QJsonDocument doc(lexis);
            content = doc.toJson(QJsonDocument::Indented);
            outputPath = yimPath + "/Lexis/" + currentOutfitName + "_exported.json";
        } else if (format == "Stand") {
            content = yimToStand(currentOutfit);
            outputPath = yimPath + "/Stand/" + currentOutfitName + "_exported.txt";
        } else {
            QJsonDocument doc(currentOutfit);
            content = doc.toJson(QJsonDocument::Indented);
            outputPath = yimPath + "/YimMenu/" + currentOutfitName + "_exported.json";
        }
        
        QFile file(outputPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(content.toUtf8());
            file.close();
            QMessageBox::information(this, "Success", "Outfit exported to:\n" + outputPath);
        } else {
            QMessageBox::critical(this, "Error", "Failed to export outfit");
        }
    }
    
    void setupUI() {
        QHBoxLayout* mainLayout = new QHBoxLayout(this);
        
        QWidget* leftPanel = new QWidget(this);
        leftPanel->setMaximumWidth(300);
        QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
        
        playerNameLabel = new QLabel("Player: Loading...", this);
        playerNameLabel->setStyleSheet("color: #667eea; font-size: 16px; font-weight: bold; padding: 10px;");
        leftLayout->addWidget(playerNameLabel);
        
        QLabel* outfitsLabel = new QLabel("Saved Outfits:", this);
        outfitsLabel->setStyleSheet("color: #fff; font-size: 14px; font-weight: bold; padding: 5px;");
        leftLayout->addWidget(outfitsLabel);
        
        outfitList = new QListWidget(this);
        outfitList->setStyleSheet(
            "QListWidget { background: #2a2a2a; color: #fff; border: 2px solid #444; border-radius: 8px; padding: 5px; }"
            "QListWidget::item { padding: 8px; border-radius: 4px; }"
            "QListWidget::item:selected { background: #667eea; }"
            "QListWidget::item:hover { background: #3a3a3a; }"
        );
        connect(outfitList, &QListWidget::itemClicked, this, &OutfitEditorTab::onOutfitSelected);
        leftLayout->addWidget(outfitList);
        
        QPushButton* refreshBtn = new QPushButton("🔄 Refresh List", this);
        refreshBtn->setStyleSheet(
            "QPushButton { background: #667eea; color: white; border: none; border-radius: 8px; padding: 10px; font-weight: bold; }"
            "QPushButton:hover { background: #7e8ef5; }"
        );
        connect(refreshBtn, &QPushButton::clicked, this, &OutfitEditorTab::loadPlayerData);
        leftLayout->addWidget(refreshBtn);
        
        mainLayout->addWidget(leftPanel);
        
        QWidget* rightPanel = new QWidget(this);
        QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
        
        QLabel* editorTitle = new QLabel("Outfit Editor", this);
        editorTitle->setStyleSheet("color: #fff; font-size: 18px; font-weight: bold; padding: 10px;");
        rightLayout->addWidget(editorTitle);
        
        QHBoxLayout* nameLayout = new QHBoxLayout();
        QLabel* nameLabel = new QLabel("Outfit Name:", this);
        nameLabel->setStyleSheet("color: #fff; font-weight: bold;");
        outfitNameEdit = new QLineEdit(this);
        outfitNameEdit->setStyleSheet("QLineEdit { background: #2a2a2a; color: #fff; border: 2px solid #444; padding: 8px; border-radius: 6px; }");
        QPushButton* renameBtn = new QPushButton("✏️ Rename", this);
        renameBtn->setStyleSheet(
            "QPushButton { background: #764ba2; color: white; border: none; border-radius: 6px; padding: 8px 15px; font-weight: bold; }"
            "QPushButton:hover { background: #8e5bb8; }"
        );
        connect(renameBtn, &QPushButton::clicked, this, &OutfitEditorTab::renameOutfit);
        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(outfitNameEdit, 1);
        nameLayout->addWidget(renameBtn);
        rightLayout->addLayout(nameLayout);
        
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet("QScrollArea { border: none; background: #1a1a1a; }");
        
        QWidget* scrollWidget = new QWidget();
        QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
        
        QLabel* compLabel = new QLabel("Components:", this);
        compLabel->setStyleSheet("color: #667eea; font-size: 16px; font-weight: bold; margin-top: 10px;");
        scrollLayout->addWidget(compLabel);
        
        componentsLayout = new QVBoxLayout();
        scrollLayout->addLayout(componentsLayout);
        
        QLabel* propsLabel = new QLabel("Props:", this);
        propsLabel->setStyleSheet("color: #764ba2; font-size: 16px; font-weight: bold; margin-top: 20px;");
        scrollLayout->addWidget(propsLabel);
        
        propsLayout = new QVBoxLayout();
        scrollLayout->addLayout(propsLayout);
        
        scrollLayout->addStretch();
        scrollArea->setWidget(scrollWidget);
        rightLayout->addWidget(scrollArea);
        
        QHBoxLayout* exportLayout = new QHBoxLayout();
        QLabel* exportLabel = new QLabel("Export to:", this);
        exportLabel->setStyleSheet("color: #fff; font-weight: bold;");
        exportFormatCombo = new QComboBox(this);
        exportFormatCombo->addItems({"YimMenu", "Cherax", "Lexis", "Stand"});
        exportFormatCombo->setStyleSheet(
            "QComboBox { background: #2a2a2a; color: #fff; border: 2px solid #444; padding: 8px; border-radius: 6px; }"
            "QComboBox::drop-down { border: none; }"
            "QComboBox QAbstractItemView { background: #2a2a2a; color: #fff; selection-background-color: #667eea; }"
        );
        QPushButton* exportBtn = new QPushButton("📤 Export Outfit", this);
        exportBtn->setStyleSheet(
            "QPushButton { background: #4CAF50; color: white; border: none; border-radius: 6px; padding: 10px 20px; font-weight: bold; }"
            "QPushButton:hover { background: #5dbf60; }"
        );
        connect(exportBtn, &QPushButton::clicked, this, &OutfitEditorTab::exportOutfit);
        exportLayout->addWidget(exportLabel);
        exportLayout->addWidget(exportFormatCombo, 1);
        exportLayout->addWidget(exportBtn);
        rightLayout->addLayout(exportLayout);
        
        statusLabel = new QLabel("Select an outfit to begin editing", this);
        statusLabel->setStyleSheet("color: #888; font-size: 12px; padding: 5px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        rightLayout->addWidget(statusLabel);
        
        mainLayout->addWidget(rightPanel, 1);
    }
    
private:
    QLabel* playerNameLabel;
    QListWidget* outfitList;
    QLineEdit* outfitNameEdit;
    QComboBox* exportFormatCombo;
    QVBoxLayout* componentsLayout;
    QVBoxLayout* propsLayout;
    QLabel* statusLabel;
    
    QMap<int, QSpinBox*> componentSpinBoxes;
    QMap<int, QSpinBox*> textureSpinBoxes;
    QMap<int, QSpinBox*> propDrawableSpinBoxes;
    QMap<int, QSpinBox*> propTextureSpinBoxes;
    
    QString currentOutfitName;
    QJsonObject currentOutfit;
};

class VehicleConverterTab : public QWidget {
    Q_OBJECT
public:
    explicit VehicleConverterTab(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);
        
        QLabel* lockIcon = new QLabel("🔒", this);
        lockIcon->setAlignment(Qt::AlignCenter);
        lockIcon->setStyleSheet("font-size: 64px;");
        layout->addWidget(lockIcon);
        
        QLabel* title = new QLabel("Vehicle Converter", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("color: #fff; font-size: 28px; font-weight: bold; margin-top: 20px;");
        layout->addWidget(title);
        
        QLabel* message = new QLabel("Coming Soon", this);
        message->setAlignment(Qt::AlignCenter);
        message->setStyleSheet("color: #667eea; font-size: 20px; font-weight: bold; margin-top: 10px;");
        layout->addWidget(message);
        
        QLabel* description = new QLabel(
            "This feature is currently under development.\n"
            "Vehicle conversion will be available in a future update.\n\n"
            "Stay tuned!",
            this
        );
        description->setAlignment(Qt::AlignCenter);
        description->setStyleSheet("color: #aaa; font-size: 14px; margin-top: 20px;");
        layout->addWidget(description);
        
        layout->addStretch();
    }
};

class ConverterTab : public QWidget {
    Q_OBJECT
public:
    explicit ConverterTab(QWidget* parent = nullptr) : QWidget(parent) {
        setupOutputDirectories();
        setupUI();
    }
    
private slots:
    void handleFilesLoad(const QStringList& filePaths) {
        currentFiles = filePaths;
        
        if (filePaths.isEmpty()) return;
        
        if (filePaths.size() == 1) {
            OutfitFormat fmt = detectFormat(filePaths[0]);
            QString formatStr = getFormatName(fmt);
            QString icon = getFormatIcon(fmt);
            QString color = getFormatColor(fmt);
            
            detectedFormatLabel->setText(QString("%1 <b>%2</b> format detected").arg(icon, formatStr));
            detectedFormatLabel->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: normal; padding: 5px;").arg(color));
            statusLabel->setText("✓ Loaded: " + QFileInfo(filePaths[0]).fileName());
            statusLabel->setStyleSheet("color: #4CAF50; font-size: 13px; padding: 10px;");
            
            convertBtn->setEnabled(fmt != OutfitFormat::Unknown);
        } else {
            detectedFormatLabel->setText(QString("📦 <b>%1 files</b> loaded for batch conversion").arg(filePaths.size()));
            detectedFormatLabel->setStyleSheet("color: #667eea; font-size: 14px; font-weight: normal; padding: 5px;");
            statusLabel->setText(QString("✓ Loaded %1 files").arg(filePaths.size()));
            statusLabel->setStyleSheet("color: #4CAF50; font-size: 13px; padding: 10px;");
            convertBtn->setEnabled(true);
        }
    }
    
    void updateMode() {
        bool isBatch = batchModeRadio->isChecked();
        dropZone->setBatchMode(isBatch);
        
        if (isBatch) {
            convertBtn->setText("🔄 Batch Convert All to YimMenu");
        } else {
            convertBtn->setText("🔄 Convert to YimMenu");
        }
    }
    
    void onManualModeChanged(bool isManual) {
        if (isManual) {
            detectedFormatLabel->setText("📝 Manual format selection enabled");
            detectedFormatLabel->setStyleSheet("color: #f093fb; font-size: 14px; font-weight: normal; padding: 5px;");
        }
    }
    
    void performConversion() {
        if (currentFiles.isEmpty()) return;
        
        QProgressDialog progress("Converting files...", "Cancel", 0, currentFiles.size(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumDuration(0);
        
        int successCount = 0;
        int errorCount = 0;
        QStringList errorFiles;
        
        for (int i = 0; i < currentFiles.size(); ++i) {
            if (progress.wasCanceled()) break;
            
            progress.setValue(i);
            progress.setLabelText(QString("Converting %1 of %2...\n%3")
                .arg(i + 1).arg(currentFiles.size())
                .arg(QFileInfo(currentFiles[i]).fileName()));
            
            OutfitFormat fmt = detectFormat(currentFiles[i]);
            
            if (fmt == OutfitFormat::Unknown) {
                errorCount++;
                errorFiles.append(QFileInfo(currentFiles[i]).fileName());
                continue;
            }
            
            QString yimJson = convertToYim(currentFiles[i], fmt);
            
            if (yimJson.isEmpty()) {
                errorCount++;
                errorFiles.append(QFileInfo(currentFiles[i]).fileName());
                continue;
            }
            
            if (saveYimFile(yimJson, currentFiles[i], fmt)) {
                successCount++;
            } else {
                errorCount++;
                errorFiles.append(QFileInfo(currentFiles[i]).fileName());
            }
        }
        
        progress.setValue(currentFiles.size());
        
        QString message = QString("Conversion Complete!\n\n"
                                 "✓ Successfully converted: %1\n"
                                 "✗ Failed: %2\n\n"
                                 "Files saved to:\n%3")
                        .arg(successCount).arg(errorCount).arg(documentsPath);
        
        if (!errorFiles.isEmpty()) {
            message += "\n\nFailed files:\n" + errorFiles.join("\n");
        }
        
        QMessageBox::information(this, "Conversion Complete", message);
        
        statusLabel->setText(QString("✓ Converted %1 files to YimMenu format").arg(successCount));
        statusLabel->setStyleSheet("color: #4CAF50; font-size: 13px; padding: 10px;");
    }
    
    QString convertToYim(const QString& filePath, OutfitFormat fmt) {
        // Check if manual mode is enabled
        if (manualSelector->isManualMode()) {
            QString sourceFormat = manualSelector->getSourceFormat();
            QString targetFormat = manualSelector->getTargetFormat();
            
            // Convert source format string to OutfitFormat enum
            if (sourceFormat == "Cherax") fmt = OutfitFormat::Cherax;
            else if (sourceFormat == "YimMenu") fmt = OutfitFormat::YimMenu;
            else if (sourceFormat == "Lexis") fmt = OutfitFormat::Lexis;
            else if (sourceFormat == "Stand") fmt = OutfitFormat::Stand;
        }
        
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            return QString();
        }
        
        QByteArray data = file.readAll();
        file.close();
        
        QJsonObject yimObj;
        
        switch (fmt) {
            case OutfitFormat::Cherax: {
                QJsonDocument doc = QJsonDocument::fromJson(data);
                yimObj = cheraxToYim(doc.object());
                break;
            }
            case OutfitFormat::YimMenu: {
                return QString::fromUtf8(data);
            }
            case OutfitFormat::Lexis: {
                QJsonDocument doc = QJsonDocument::fromJson(data);
                yimObj = lexisToYim(doc.object());
                break;
            }
            case OutfitFormat::Stand: {
                return standToYim(QString::fromUtf8(data));
            }
            default:
                return QString();
        }
        
        QJsonDocument doc(yimObj);
        return QString(doc.toJson(QJsonDocument::Indented));
    }
    
    bool saveYimFile(const QString& jsonContent, const QString& originalPath, OutfitFormat sourceFormat) {
        QFileInfo info(originalPath);
        QString baseName = info.baseName();
        
        QString yimPath = documentsPath + "/OutfitConverter/YimMenu";
        
        QDir dir;
        if (!dir.exists(yimPath)) {
            dir.mkpath(yimPath);
        }
        
        QString outputFile = yimPath + "/" + baseName + "_converted.json";
        
        int counter = 1;
        while (QFile::exists(outputFile)) {
            outputFile = yimPath + "/" + baseName + "_converted_" + QString::number(counter) + ".json";
            counter++;
        }
        
        QFile file(outputFile);
        if (!file.open(QIODevice::WriteOnly)) {return false;
        }
        
        file.write(jsonContent.toUtf8());
        file.close();
        
        return true;
    }
    
    void setupOutputDirectories() {
        documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        
        QDir dir;
        QStringList folders = {"Cherax", "YimMenu", "Lexis", "Stand"};
        
        for (const QString& folder : folders) {
            QString path = documentsPath + "/OutfitConverter/" + folder;
            if (!dir.exists(path)) {
                dir.mkpath(path);
            }
        }
    }
    
    QString getFormatName(OutfitFormat fmt) {
        switch (fmt) {
            case OutfitFormat::Cherax: return "Cherax";
            case OutfitFormat::YimMenu: return "YimMenu";
            case OutfitFormat::Lexis: return "Lexis";
            case OutfitFormat::Stand: return "Stand";
            default: return "Unknown";
        }
    }
    
    QString getFormatIcon(OutfitFormat fmt) {
        switch (fmt) {
            case OutfitFormat::Cherax: return "🔷";
            case OutfitFormat::YimMenu: return "🟣";
            case OutfitFormat::Lexis: return "🔶";
            case OutfitFormat::Stand: return "📝";
            default: return "❓";
        }
    }
    
    QString getFormatColor(OutfitFormat fmt) {
        switch (fmt) {
            case OutfitFormat::Cherax: return "#667eea";
            case OutfitFormat::YimMenu: return "#764ba2";
            case OutfitFormat::Lexis: return "#f093fb";
            case OutfitFormat::Stand: return "#4facfe";
            default: return "#ff6b6b";
        }
    }
    
    QPushButton* createStyledButton(const QString& text, const QString& color) {
        QPushButton* btn = new QPushButton(text, this);
        btn->setMinimumHeight(50);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton {"
            "  background: %1;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 8px;"
            "  font-size: 15px;"
            "  font-weight: bold;"
            "  padding: 15px;"
            "}"
            "QPushButton:hover {"
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 %1, stop:1 %2);"
            "}"
            "QPushButton:pressed {"
            "  background: #555;"
            "}"
            "QPushButton:disabled {"
            "  background: #333;"
            "  color: #666;"
            "}"
        ).arg(color).arg(color));
        
        return btn;
    }
    
    void setupUI() {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(20);
        mainLayout->setContentsMargins(30, 30, 30, 30);
        
        dropZone = new DropZone(this);
        connect(dropZone, &DropZone::filesDropped, this, &ConverterTab::handleFilesLoad);
        mainLayout->addWidget(dropZone);
        
        QGroupBox* detectionBox = new QGroupBox("Auto-Detection", this);
        detectionBox->setStyleSheet(
            "QGroupBox { background: #2a2a2a; border: 2px solid #444; border-radius: 12px; "
            "margin-top: 10px; padding-top: 20px; color: #fff; font-size: 14px; font-weight: bold; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; }"
        );
        
        QVBoxLayout* detectionLayout = new QVBoxLayout(detectionBox);
        detectedFormatLabel = new QLabel("No files loaded", this);
        detectedFormatLabel->setAlignment(Qt::AlignCenter);
        detectedFormatLabel->setStyleSheet("color: #888; font-size: 13px; font-weight: normal; padding: 5px;");
        detectionLayout->addWidget(detectedFormatLabel);
        mainLayout->addWidget(detectionBox);
        
        // Add manual format selector
        manualSelector = new ManualFormatSelector(this);
        connect(manualSelector, &ManualFormatSelector::modeChanged, this, &ConverterTab::onManualModeChanged);
        mainLayout->addWidget(manualSelector);
        
        QGroupBox* modeBox = new QGroupBox("Conversion Mode", this);
        modeBox->setStyleSheet(
            "QGroupBox { background: #2a2a2a; border: 2px solid #444; border-radius: 12px; "
            "margin-top: 10px; padding-top: 20px; color: #fff; font-size: 14px; font-weight: bold; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; }"
            "QRadioButton { color: #fff; font-size: 13px; font-weight: normal; spacing: 8px; }"
            "QRadioButton::indicator { width: 18px; height: 18px; }"
            "QRadioButton::indicator::unchecked { border: 2px solid #666; border-radius: 9px; background: #1a1a1a; }"
            "QRadioButton::indicator::checked { border: 2px solid #667eea; border-radius: 9px; background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #667eea, stop:1 #667eea); }"
        );
        
        QVBoxLayout* modeLayout = new QVBoxLayout(modeBox);
        singleModeRadio = new QRadioButton("Single File Mode - Convert one file at a time", this);
        batchModeRadio = new QRadioButton("Batch Mode - Convert multiple files at once", this);
        
        singleModeRadio->setChecked(true);
        
        buttonGroup = new QButtonGroup(this);
        buttonGroup->addButton(singleModeRadio);
        buttonGroup->addButton(batchModeRadio);
        
        connect(singleModeRadio, &QRadioButton::toggled, this, &ConverterTab::updateMode);
        
        modeLayout->addWidget(singleModeRadio);
        modeLayout->addWidget(batchModeRadio);
        mainLayout->addWidget(modeBox);
        
        convertBtn = createStyledButton("🔄 Convert to YimMenu", "#667eea");
        connect(convertBtn, &QPushButton::clicked, this, &ConverterTab::performConversion);
        convertBtn->setEnabled(false);
        mainLayout->addWidget(convertBtn);
        
        statusLabel = new QLabel("Load file(s) to begin", this);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet("color: #888; font-size: 13px; padding: 10px;");
        mainLayout->addWidget(statusLabel);
    }
    
private:
    DropZone* dropZone;
    QPushButton* convertBtn;
    QLabel* statusLabel;
    QLabel* detectedFormatLabel;
    QRadioButton* singleModeRadio;
    QRadioButton* batchModeRadio;
    QButtonGroup* buttonGroup;
    QStringList currentFiles;
    QString documentsPath;
    ManualFormatSelector* manualSelector;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Outfit Converter Pro - Multi-Format Support");
        setMinimumSize(1100, 900);
        
        QWidget* central = new QWidget(this);
        setCentralWidget(central);
        
        QVBoxLayout* mainLayout = new QVBoxLayout(central);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        
        QLabel* title = new QLabel("Outfit Converter Pro", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 28px; font-weight: bold; color: #fff; margin-bottom: 5px;");
        mainLayout->addWidget(title);
        
        QLabel* subtitle = new QLabel("Convert, Edit & Manage - Cherax, YimMenu, Lexis & Stand", this);
        subtitle->setAlignment(Qt::AlignCenter);
        subtitle->setStyleSheet("font-size: 14px; color: #aaa; margin-bottom: 5px;");
        mainLayout->addWidget(subtitle);
        
        QLabel* credits = new QLabel("Enhanced by @sizrox", this);
        credits->setAlignment(Qt::AlignCenter);
        credits->setStyleSheet("font-size: 11px; color: #667eea; margin-bottom: 15px; font-style: italic;");
        mainLayout->addWidget(credits);
        
        QTabWidget* tabWidget = new QTabWidget(this);
        tabWidget->setStyleSheet(
            "QTabWidget::pane { border: 2px solid #444; border-radius: 8px; background: #1a1a1a; }"
            "QTabBar::tab { background: #2a2a2a; color: #aaa; padding: 12px 24px; border: 2px solid #444; "
            "border-bottom:none; margin-right: 2px; border-top-left-radius: 8px; border-top-right-radius: 8px; }"
            "QTabBar::tab:selected { background: #667eea; color: white; font-weight: bold; }"
            "QTabBar::tab:hover { background: #3a3a3a; }"
        );
        
        tabWidget->addTab(new ConverterTab(this), "🔄 Converter");
        tabWidget->addTab(new OutfitEditorTab(this), "✏️ Outfit Editor");
        tabWidget->addTab(new VehicleConverterTab(this), "🚗 Vehicle Converter");
        
        mainLayout->addWidget(tabWidget);
        
        setStyleSheet("QMainWindow { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1a1a1a, stop:1 #0d0d0d); }");
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Outfit Converter Pro");
    app.setApplicationVersion("3.0");
    app.setOrganizationName("sizrox");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

#include "main.moc"
