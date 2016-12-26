#include <frm/Spline.h>

#include <frm/interpolation.h>
#include <frm/math.h>

#include <apt/Json.h>

#include <frm/Im3d.h>
#include <imgui/imgui.h>

using namespace frm;
using namespace apt;

// PUBLIC

vec3 SplinePath::evaluate(float _t) const
{
	int i = findSegment(_t);
	const Segment& seg = m_segs[i];
	float t = (_t - seg.m_beg) / seg.m_len;
	int p0, p1, p2, p3;
	getClampIndices(i, p0, p1, p2, p3);
	return itpl(m_data[p0], m_data[p1], m_data[p2], m_data[p3], t);
}

void SplinePath::append(const vec3& _position)
{
	m_data.push_back(_position);
}

void SplinePath::build()
{
	if (m_data.empty()) {
		return;
	}
	m_segs.clear();
	m_segs.resize(m_data.size() - 1);

 // segment beg/length, total length
	m_length = 0.0f;
	for (int i = 0, n = (int)m_segs.size(); i < n; ++i) {
		int p0, p1, p2, p3;
		getClampIndices(i, p0, p1, p2, p3);
		m_segs[i].m_len = arclen(m_data[p0], m_data[p1], m_data[p2], m_data[p3]);
		m_length += m_segs[i].m_len;
		m_segs[i].m_beg = i == 0 ? 0.0f : m_segs[i - 1].m_beg + m_segs[i - 1].m_len;
	}

 // normalize
	for (int i = 0, n = (int)m_segs.size(); i < n; ++i) {
		m_segs[i].m_beg /= m_length;
		m_segs[i].m_len /= m_length;
	}
}

void SplinePath::edit()
{
	static const Im3d::Color kColorPath   = Im3d::Color(0.0f, 1.0f, 0.2f, 0.8f);
	static const Im3d::Color kColorPoints = Im3d::Color(1.0f, 1.0f, 1.0f, 1.0f);
	static const int kPathDetail = 64;

	Im3d::PushDrawState();
		Im3d::SetColor(kColorPath);
		Im3d::SetSize(2.0f);
		Im3d::BeginLineStrip();
			for (int i = 0, n = (int)m_data.size() - 1; i < n; ++i) {
				for (int j = 0; j < kPathDetail; ++j) {
					float t = (float)j / (float)kPathDetail;
					int p0, p1, p2, p3;
					getClampIndices(i, p0, p1, p2, p3);
					vec3 p = itpl(m_data[p0], m_data[p1], m_data[p2], m_data[p3], t);
					Im3d::Vertex(p);
				}
			}
		Im3d::End();

		Im3d::SetSize(4.0f);
		Im3d::SetColor(kColorPoints);
		Im3d::BeginPoints();
			for (int i = 0, n = (int)m_data.size(); i < n; ++i) {
				Im3d::Vertex(m_data[i]);
			 // \todo edit handles here
				//if (Im3d::GizmoPosition("SplinePath_EditData", &m_data[1])) {
				//	build();
				//}
			}
		Im3d::End();

		static float s_t = 0.0f;
		static vec3 pp = vec3(0.0f);
		vec3 p = evaluate(s_t);
		ImGui::Text("Velocity %f", length(p - pp));
		pp = p;
		s_t += (1.0f/60.0f) / 8.0f;
		if (s_t > 1.0f) s_t = 0.0f;
		ImGui::SliderFloat("T", &s_t, 0.0f, 1.0f);
		Im3d::SetColor(1.0f, 0.0f, 1.0f, 1.0f);
		Im3d::SetSize(16.0f);
		Im3d::BeginPoints();
			Im3d::Vertex(p);
		Im3d::End();

	Im3d::PopDrawState();
	
	ImGui::Text("Path Length: %f", m_length);

}

// PRIVATE

int SplinePath::findSegment(float _t) const
{
	int lo = 0, hi = (int)m_segs.size() - 1;
	while (hi - lo > 1) {
		int mid = (hi + lo) / 2;
		if (_t > m_segs[mid].m_beg) {
			lo = mid;
		} else {
			hi = mid;
		}
	}
	if (_t > m_segs[hi].m_beg) {
		return hi;
	}
	return lo;
}

vec3 SplinePath::itpl(const vec3& _p0, const vec3& _p1, const vec3& _p2, const vec3& _p3, float _t)
{
	return cuberp(_p0, _p1, _p2, _p3, _t);
}

float SplinePath::arclen(
	const vec3& _p0, const vec3& _p1, const vec3& _p2, const vec3& _p3,
	float _threshold,
	float _dbeg, float _dmid, float _dend,
	float _step
	)
{
	vec3 beg = itpl(_p0, _p1, _p2, _p3, _dbeg);
	vec3 mid = itpl(_p0, _p1, _p2, _p3, _dmid);
	vec3 end = itpl(_p0, _p1, _p2, _p3, _dend);

 // \todo length2 here?
	float a = length(mid - beg);
	float b = length(end - mid);
	float c = length(end - beg);
	if (_step < FLT_EPSILON || (a + b) - c < _threshold) {
		return c;
	}

	return 
		arclen(_p0, _p1, _p2, _p3, _threshold, _dbeg, _dmid - _step, _dmid, _step * 0.5f) +
		arclen(_p0, _p1, _p2, _p3, _threshold, _dmid, _dmid + _step, _dend, _step * 0.5f)
		;
}

void SplinePath::getClampIndices(int _i, int& p0_, int& p1_, int& p2_, int& p3_) const
{
	p0_ = APT_MAX(_i - 1, 0);
	p1_ = _i;
	p2_ = APT_MIN(_i + 1, (int)m_data.size() - 1);
	p3_ = APT_MIN(_i + 2, (int)m_data.size() - 1);
}
