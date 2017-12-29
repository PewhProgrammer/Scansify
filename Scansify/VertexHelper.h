#ifndef VERTEXHELPER_H
#define VERTEXHELPER_H

#include <QVector3D>

class VertexHelper
{
public:
	// Constructors
	Q_DECL_CONSTEXPR VertexHelper();
	Q_DECL_CONSTEXPR explicit VertexHelper(const QVector3D &position);
	Q_DECL_CONSTEXPR VertexHelper(const QVector3D &position, const QVector3D &color);

	// Accessors / Mutators
	Q_DECL_CONSTEXPR const QVector3D& position() const;
	Q_DECL_CONSTEXPR const QVector3D& color() const;
	void setPosition(const QVector3D& position);
	void setColor(const QVector3D& color);

	// OpenGL Helpers
	static const int PositionTupleSize = 3;
	static const int ColorTupleSize = 3;
	static Q_DECL_CONSTEXPR int positionOffset();
	static Q_DECL_CONSTEXPR int colorOffset();
	static Q_DECL_CONSTEXPR int stride();

private:
	QVector3D m_position;
	QVector3D m_color;
};

/*******************************************************************************
* Inline Implementation
******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(VertexHelper, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline VertexHelper::VertexHelper() {}
Q_DECL_CONSTEXPR inline VertexHelper::VertexHelper(const QVector3D &position) : m_position(position) {}
Q_DECL_CONSTEXPR inline VertexHelper::VertexHelper(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& VertexHelper::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& VertexHelper::color() const { return m_color; }
void inline VertexHelper::setPosition(const QVector3D& position) { m_position = position; }
void inline VertexHelper::setColor(const QVector3D& color) { m_color = color; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int VertexHelper::positionOffset() { return offsetof(VertexHelper, m_position); }
Q_DECL_CONSTEXPR inline int VertexHelper::colorOffset() { return offsetof(VertexHelper, m_color); }
Q_DECL_CONSTEXPR inline int VertexHelper::stride() { return sizeof(VertexHelper); }

#endif // VERTEX_H
