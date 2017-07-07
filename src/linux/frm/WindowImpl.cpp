#include <frm/Window.h>

#include <frm/def.h>
#include <frm/Input.h>

#include <apt/log.h>
#include <apt/platform.h>
#include <apt/win.h>

using namespace frm;

// Much of this is duplicated from InputImpl
static Keyboard::Key ButtonFromVk(WPARAM _vk, LPARAM _lparam)
{
 // map special cases
	UINT sc = (_lparam & 0x00ff0000) >> 16;
    bool e0 = (_lparam & 0x01000000) != 0;
	if (_vk == VK_SHIFT) {
		_vk = MapVirtualKey(sc, MAPVK_VSC_TO_VK_EX);
	}

	if (_vk >= 0x41 && _vk <= 0x5A) {
	 // alpha
		return (Keyboard::Key)(Keyboard::Key_A + _vk - 0x41);
	} else if (_vk >= 0x30 && _vk <= 0x39) {
	 // numeric
		return _vk == 0x30 ? Keyboard::Key_0 : (Keyboard::Key)(Keyboard::Key_1 + _vk - 0x31);
	} else if (_vk >= VK_NUMPAD0 && _vk <= VK_NUMPAD9) {
	 // numpad numeric
		return (Keyboard::Key)(Keyboard::Key_Numpad0 + _vk - VK_NUMPAD0);
	} else if (_vk >= VK_F1 && _vk <= VK_F12) {
	 // F keys
		return (Keyboard::Key)(Keyboard::Key_F1 + _vk - VK_F1);
	} else {
	 // all other keys
		switch (_vk) {
			case VK_CONTROL:     return e0 ? Keyboard::Key_RCtrl       : Keyboard::Key_LCtrl;
			case VK_MENU:        return e0 ? Keyboard::Key_RAlt        : Keyboard::Key_LAlt;
			case VK_RETURN:      return e0 ? Keyboard::Key_NumpadEnter : Keyboard::Key_Return;
			case VK_INSERT:      return e0 ? Keyboard::Key_Insert      : Keyboard::Key_Numpad0;
			case VK_END:         return e0 ? Keyboard::Key_End         : Keyboard::Key_Numpad1;
			case VK_DOWN:        return e0 ? Keyboard::Key_Down        : Keyboard::Key_Numpad2;
			case VK_NEXT:        return e0 ? Keyboard::Key_PageDown    : Keyboard::Key_Numpad3;
			case VK_LEFT:        return e0 ? Keyboard::Key_Left        : Keyboard::Key_Numpad4;
			case VK_CLEAR:       return e0 ? Keyboard::Key_Clear       : Keyboard::Key_Numpad5;
			case VK_RIGHT:       return e0 ? Keyboard::Key_Right       : Keyboard::Key_Numpad6;
			case VK_HOME:        return e0 ? Keyboard::Key_Home        : Keyboard::Key_Numpad7;
			case VK_UP:          return e0 ? Keyboard::Key_Up          : Keyboard::Key_Numpad8;
			case VK_PRIOR:       return e0 ? Keyboard::Key_PageUp      : Keyboard::Key_Numpad9;
			case VK_DELETE:      return e0 ? Keyboard::Key_Delete      : Keyboard::Key_NumpadPeriod;
			
		 	case VK_LSHIFT:      return Keyboard::Key_LShift;
			case VK_RSHIFT:      return Keyboard::Key_RShift;
			case VK_ESCAPE:      return Keyboard::Key_Escape;
			case VK_PAUSE:       return Keyboard::Key_Pause;
			case VK_SNAPSHOT:    return Keyboard::Key_PrintScreen;
			case VK_CAPITAL:     return Keyboard::Key_CapsLock;
			case VK_NUMLOCK:     return Keyboard::Key_NumLock;
			case VK_DECIMAL:     return Keyboard::Key_NumpadPeriod; 
			case VK_ADD:         return Keyboard::Key_NumpadPlus;   
			case VK_SUBTRACT:    return Keyboard::Key_NumpadMinus;  
			case VK_DIVIDE:      return Keyboard::Key_NumpadDivide; 
			case VK_MULTIPLY:    return Keyboard::Key_NumpadMultiply;
			case VK_SCROLL:      return Keyboard::Key_ScrollLock;
			case VK_SPACE:       return Keyboard::Key_Space;
			case VK_BACK:        return Keyboard::Key_Backspace;
			case VK_TAB:         return Keyboard::Key_Tab;
			case VK_OEM_PLUS:    return Keyboard::Key_Plus;
			case VK_OEM_MINUS:   return Keyboard::Key_Minus;
			case VK_OEM_PERIOD:  return Keyboard::Key_Period;
			case VK_OEM_COMMA:   return Keyboard::Key_Comma;
			case VK_OEM_1:       return Keyboard::Key_Misc0;
			case VK_OEM_2:       return Keyboard::Key_Misc1;
			case VK_OEM_3:       return Keyboard::Key_Misc2;
			case VK_OEM_4:       return Keyboard::Key_Misc3;
			case VK_OEM_5:       return Keyboard::Key_Misc4;
			case VK_OEM_6:       return Keyboard::Key_Misc5;
			case VK_OEM_7:       return Keyboard::Key_Misc6;
			case VK_OEM_8:       return Keyboard::Key_Misc7;

			default:             return Keyboard::Key_Unmapped;
		};
	}
}


/*******************************************************************************

                               Window::Impl

*******************************************************************************/

struct Window::Impl
{
	static LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
	{
		Window* window = Window::Find((void*)_hwnd);
		#define DISPATCH_CALLBACK(cbk, ...) \
			if (window->m_callbacks.m_##cbk) { \
				if (window->m_callbacks.m_##cbk(window, __VA_ARGS__)) { \
					return 0; \
				} \
			}
		if (window) {
			switch (_umsg) {
				case WM_SIZE: {
					int w = (int)LOWORD(_lparam), h = (int)HIWORD(_lparam);
					if (window->m_width != w || window->m_height != h) {
						window->m_width = w;
						window->m_height = h;
						DISPATCH_CALLBACK(OnResize, window->m_width, window->m_height);
					}
					break;
				}
				case WM_SIZING: {
					RECT* r = (RECT*)_lparam;
					int w = (int)(r->right - r->left);
					int h = (int)(r->bottom - r->top);
					if (window->m_width != w || window->m_height != h) {
						window->m_width = w;
						window->m_height = h;
						DISPATCH_CALLBACK(OnResize, window->m_width, window->m_height);
					}
					break;
				}
				case WM_SHOWWINDOW:
					if (_wparam) {
						DISPATCH_CALLBACK(OnShow);
					} else {
						DISPATCH_CALLBACK(OnHide);
					}
					break;

				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
					DISPATCH_CALLBACK(OnMouseButton, (unsigned)Mouse::Button_Left, _umsg == WM_LBUTTONDOWN);
					break;
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
					DISPATCH_CALLBACK(OnMouseButton, (unsigned)Mouse::Button_Middle, _umsg == WM_MBUTTONDOWN);
					break;
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
					DISPATCH_CALLBACK(OnMouseButton, (unsigned)Mouse::Button_Right, _umsg == WM_RBUTTONDOWN);
					break;

				case WM_MOUSEWHEEL:
					DISPATCH_CALLBACK(OnMouseWheel, (float)(GET_WHEEL_DELTA_WPARAM(_wparam)) / (float)(WHEEL_DELTA)); 
					break;

				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
					//APT_LOG_DBG("%s", Keyboard::GetButtonName(ButtonFromVk(_wparam, _lparam)));
					DISPATCH_CALLBACK(OnKey, (unsigned)ButtonFromVk(_wparam, _lparam), _umsg == WM_KEYDOWN || _umsg == WM_SYSKEYDOWN);
					break;
				case WM_CHAR:
					DISPATCH_CALLBACK(OnChar, (char)_wparam);
					break;

				default: break;
			};
		}
		#undef DISPATCH_CALLBACK
	
		switch (_umsg) {
			case WM_PAINT:
				//APT_ASSERT(false); // should be suppressed by calling ValidateRect()
				break;
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0; // prevent DefWindowProc from destroying the window
			default:
				break;
		};
	
		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}

}; // struct Window::Impl


/*******************************************************************************

                                 Window

*******************************************************************************/

// PUBLIC

Window* Window::Create(int _width, int _height, const char* _title)
{
	APT_STATIC_ASSERT(sizeof(void*) >= sizeof(HWND));
	Window* ret = new Window;
	APT_ASSERT(ret);
	ret->m_width  = _width;
	ret->m_height = _height;
	ret->m_title  = _title;

	static ATOM wndclassex = 0;
	if (wndclassex == 0) {
		WNDCLASSEX wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;// | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = Impl::WindowProc;
		wc.hInstance = GetModuleHandle(0);
		wc.lpszClassName = "WindowImpl";
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		APT_PLATFORM_VERIFY(wndclassex = RegisterClassEx(&wc));
	}

	
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	if (_width == -1 || _height == -1) {
	 // auto size; get the dimensions of the primary screen area and subtract the non-client area
		RECT r;
		APT_PLATFORM_VERIFY(SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0));
		_width  = r.right - r.left;
		_height = r.bottom - r.top;

		RECT wr = {};
		APT_PLATFORM_VERIFY(AdjustWindowRectEx(&wr, dwStyle, FALSE, dwExStyle));
		_width  -= wr.right - wr.left;
		_height -= wr.bottom - wr.top;
	}
	ret->m_width = _width;
	ret->m_height = _height;

	RECT r; r.top = 0; r.left = 0; r.bottom = _height; r.right = _width;
	APT_PLATFORM_VERIFY(AdjustWindowRectEx(&r, dwStyle, FALSE, dwExStyle));
	ret->m_handle = CreateWindowEx(
		dwExStyle, 
		MAKEINTATOM(wndclassex), 
		ret->m_title, 
		dwStyle, 
		0, 0, 
		r.right - r.left, r.bottom - r.top, 
		NULL, 
		NULL, 
		GetModuleHandle(0), 
		NULL
		);
	APT_PLATFORM_ASSERT(ret->m_handle);
	return ret;
}

void Window::Destroy(Window*& _window_)
{
	APT_ASSERT(_window_ != 0);
	APT_PLATFORM_VERIFY(DestroyWindow((HWND)_window_->m_handle));
	_window_->m_handle = 0;
	delete _window_;
	_window_ = 0;
}

bool Window::pollEvents() const
{
	MSG msg;
	while (PeekMessage(&msg, (HWND)m_handle, 0, 0, PM_REMOVE) && msg.message != WM_QUIT) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message != WM_QUIT;
}
bool Window::waitEvents() const
{
	MSG msg;
	if (GetMessage(&msg, (HWND)m_handle, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.message != WM_QUIT;
}

void Window::show() const
{
	ShowWindow((HWND)m_handle, SW_SHOW);
}
void Window::hide() const
{
	ShowWindow((HWND)m_handle, SW_HIDE);
}
void Window::maximize() const
{
	ShowWindow((HWND)m_handle, SW_MAXIMIZE);
}
void Window::minimize() const
{
	ShowWindow((HWND)m_handle, SW_MINIMIZE);
}
void Window::setPositionSize(int _x, int _y, int _width, int _height)
{
	APT_PLATFORM_VERIFY(SetWindowPos((HWND)m_handle, NULL, _x, _y, _width, _height, 0));
}

bool Window::hasFocus() const
{
	return (HWND)m_handle == GetFocus();
}
bool Window::isMinimized() const
{
	return IsIconic((HWND)m_handle) != 0;
}
bool Window::isMaximized() const
{
	return IsZoomed((HWND)m_handle) != 0;
}

void Window::getWindowRelativeCursor(int* x_, int* y_) const
{
	APT_ASSERT(x_);
	APT_ASSERT(y_);

	POINT p = {};
	APT_PLATFORM_VERIFY(GetCursorPos(&p));
	APT_PLATFORM_VERIFY(ScreenToClient((HWND)m_handle, &p));
	*x_ = (int)p.x;
	*y_ = (int)p.y;
}