#pragma once
#include <QAbstractListModel>

#include <QList>

class ComponentListItemModel : public QAbstractListModel
{
	Q_OBJECT

	public:
		ComponentListItemModel(QObject *parent = 0);
		~ComponentListItemModel();

		virtual int rowCount(QModelIndex const& parent = QModelIndex()) const override;
		virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
		
		virtual bool insertRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;
		virtual bool removeRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;

		bool setData(const QModelIndex &index, QVariant const& value, int role = Qt::EditRole);
	private:
		
		void _onRenderComponentAdded(unsigned int entityId);
		void _onRenderComponentRemoved(unsigned int entityId);

		struct Component
		{
			Component(std::string const& name, unsigned int entityId)
				: name(name), entityId(entityId) { }

			std::string		name;
			unsigned int	entityId;
		};
		QList<Component> mComponents;
};

