#include "vizelement.h"
#include "vizgraph.h"
#include "vizgroup.h"

#include "gfa/line.h"

#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QJsonDocument>

VizElement::VizElement(VizGraph* _vg, GfaLine* line) {
  vg = _vg;
  setCacheMode( QGraphicsItem::DeviceCoordinateCache );
  //setFlag(ItemSendsGeometryChanges);
  labelItem = NULL;
  
  if (line->hasTag(VIZ_OPTIONSTAG, GFA_TAG_JSON)) {
    char* styledata = line->getTag(VIZ_OPTIONSTAG, GFA_TAG_JSON)->getStringValue();
    QJsonDocument jsondata = QJsonDocument::fromJson(styledata);
    if (!jsondata.isNull()) {
      settings.fromJson(jsondata.object());
    }
  }
}

VizElement::~VizElement() {
  delete labelItem;
}

void VizElement::addGroup(VizGroup* group) {
  groups.push_back(group);
}

void VizElement::drawLabel() {
  if (!getGfaElement()->hasName())
    return;
  
  if (!labelItem) 
    labelItem = new VizElementLabel(QString::fromStdString(getGfaElement()->getName()), this);
  //labelItem.setParentItem(this);
  QString text = QString::fromStdString(getGfaElement()->getName());
  if (text != labelItem->toPlainText())
    labelItem->setPlainText(text);
  
  QRectF bounds = labelItem->boundingRect();
  labelItem->setPos(getCenterCoord());
  labelItem->moveBy(-bounds.width() / 2, -bounds.height() / 2);
  //vg->scene->addItem(labelItem);
}

const QVariant VizElement::getOption(VizGraphParam p) const {
  return settings.get(p, &vg->settings);
}
/*
void VizElement::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
  QPen mypen = pen();
  QBrush mybrush = brush();
  setBrush(QBrush(Qt::transparent));
  setPen(QPen(Qt::red, 5));
  QGraphicsPathItem::paint (painter, option, widget);
  setBrush(mybrush);
  setPen(mypen);
  QGraphicsPathItem::paint (painter, option, widget);
}*/

QVariant VizElement::itemChange(GraphicsItemChange change, const QVariant &value) {
  //cout << "lol123 " << (int)change <<  endl;
  if (change == ItemPositionHasChanged && scene()) {
    for (VizGroup* group : groups) {
      group->update();
    }
  }
  if (change == ItemSelectedHasChanged && scene()) {
    for (VizGroup* group : groups) {
      group->update(boundingRect());
    }
  }
  return QGraphicsPathItem::itemChange(change, value);
}

VizElementLabel::VizElementLabel(QString text, VizElement* _parent) : QGraphicsTextItem(text,_parent) {
  parent=_parent;
  setParentItem(parent);
  
  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  setFlags(ItemIsMovable);
  setBoundingRegionGranularity(1.0);
  QFont font;
  //font.setBold(true);
  font.setFamily(parent->getOption(VIZ_LABELFONT).toString());
  font.setPointSize(parent->getOption(VIZ_LABELFONTSIZE).toDouble());
  setFont(font);
  setDefaultTextColor(parent->getOption(VIZ_LABELCOLOR).value<QColor>());
  document()->setDocumentMargin(1.0);
  /*QTextDocument* document = new QTextDocument;
  QTextCharFormat charFormat;
  QFont font;
  //font.setBold(true);
  //font.setWeight(QFont::Black);
  font.setFamily("Arial");
  setDefaultTextColor(QColor(0,0,0));
  //font.setPointSize(16);
  charFormat.setFont(font);
  QPen outlinePen = QPen (QColor(255,255,255), 0.6, Qt::SolidLine);
  charFormat.setTextOutline(outlinePen);
  QTextCursor cursor = QTextCursor(document);
  cursor.insertText(text, charFormat);
  setDocument(document);*/
  //labelItem->setTextInteractionFlags(Qt::TextEditable);
}

void VizElementLabel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
  QTextCharFormat format;
  QPen outlinepen = QPen(parent->getOption(VIZ_LABELOUTLINECOLOR).value<QColor>(), parent->getOption(VIZ_LABELOUTLINEWIDTH).toDouble(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  format.setTextOutline (outlinepen);
  QTextCursor cursor (this->document());
  cursor.select (QTextCursor::Document);
  cursor.mergeCharFormat (format);
  QGraphicsTextItem::paint (painter, option, widget);
  format.setTextOutline (QPen (Qt::transparent));
  cursor.mergeCharFormat (format);
  QGraphicsTextItem::paint (painter, option, widget);
}