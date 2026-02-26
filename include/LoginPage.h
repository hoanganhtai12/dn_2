const char loginCode[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Đăng Nhập - Trang Web Đẹp</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Poppins', sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            overflow: hidden;
        }

        .container {
            position: relative;
            width: 100%;
            height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .login-box {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border-radius: 20px;
            padding: 40px;
            width: 400px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            border: 1px solid rgba(255, 255, 255, 0.2);
            position: relative;
            z-index: 10;
            animation: slideUp 0.8s ease-out;
        }

        @keyframes slideUp {
            from {
                opacity: 0;
                transform: translateY(50px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .login-header {
            text-align: center;
            margin-bottom: 30px;
        }

        .logo {
            font-size: 60px;
            color: #667eea;
            margin-bottom: 20px;
        }

        .login-header h2 {
            color: #333;
            font-weight: 600;
            margin-bottom: 8px;
            font-size: 28px;
        }

        .login-header p {
            color: #666;
            font-size: 14px;
        }

        .input-group {
            margin-bottom: 20px;
        }

        .input-field {
            position: relative;
            display: flex;
            align-items: center;
        }

        .input-field i {
            position: absolute;
            left: 15px;
            color: #667eea;
            font-size: 16px;
            z-index: 2;
        }

        .input-field input {
            width: 100%;
            padding: 15px 15px 15px 45px;
            border: 2px solid #e1e1e1;
            border-radius: 12px;
            font-size: 16px;
            background: #f8f9fa;
            transition: all 0.3s ease;
            outline: none;
        }

        .input-field input:focus {
            border-color: #667eea;
            background: white;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }

        .toggle-password {
            position: absolute !important;
            right: 15px !important;
            left: auto !important;
            cursor: pointer;
            color: #999 !important;
            transition: color 0.3s ease;
        }

        .toggle-password:hover {
            color: #667eea !important;
        }

        .form-options {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 25px;
            font-size: 14px;
        }

        .remember-me {
            display: flex;
            align-items: center;
            cursor: pointer;
            color: #666;
        }

        .remember-me input {
            display: none;
        }

        .checkmark {
            width: 18px;
            height: 18px;
            border: 2px solid #e1e1e1;
            border-radius: 4px;
            margin-right: 8px;
            position: relative;
            transition: all 0.3s ease;
        }

        .remember-me input:checked + .checkmark {
            background: #667eea;
            border-color: #667eea;
        }

        .remember-me input:checked + .checkmark::after {
            content: '\f00c';
            font-family: 'Font Awesome 6 Free';
            font-weight: 900;
            position: absolute;
            color: white;
            font-size: 10px;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }

        .forgot-password {
            color: #667eea;
            text-decoration: none;
            transition: color 0.3s ease;
        }

        .forgot-password:hover {
            color: #5a6fd8;
        }

        .login-btn {
            width: 100%;
            padding: 15px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 12px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
            margin-bottom: 25px;
        }

        .login-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 25px rgba(102, 126, 234, 0.3);
        }

        .login-btn:active {
            transform: translateY(0);
        }

        .divider {
            text-align: center;
            margin: 25px 0;
            position: relative;
            color: #666;
            font-size: 14px;
        }

        .divider::before {
            content: '';
            position: absolute;
            top: 50%;
            left: 0;
            right: 0;
            height: 1px;
            background: #e1e1e1;
            z-index: 1;
        }

        .divider span {
            background: rgba(255, 255, 255, 0.95);
            padding: 0 20px;
            position: relative;
            z-index: 2;
        }

        .social-login {
            display: flex;
            gap: 15px;
            margin-bottom: 25px;
        }

        .social-btn {
            flex: 1;
            padding: 12px;
            border: 2px solid #e1e1e1;
            border-radius: 12px;
            background: white;
            color: #666;
            font-size: 14px;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 8px;
        }

        .social-btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
        }

        .social-btn.google:hover {
            border-color: #db4437;
            color: #db4437;
        }

        .social-btn.facebook:hover {
            border-color: #4267B2;
            color: #4267B2;
        }

        .signup-link {
            text-align: center;
            color: #666;
            font-size: 14px;
        }

        .signup-link a {
            color: #667eea;
            text-decoration: none;
            font-weight: 600;
            transition: color 0.3s ease;
        }

        .signup-link a:hover {
            color: #5a6fd8;
        }

        .background-animation {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            overflow: hidden;
            z-index: 1;
        }

        .shape {
            position: absolute;
            border-radius: 50%;
            background: rgba(255, 255, 255, 0.1);
            animation: float 6s ease-in-out infinite;
        }

        .shape-1 {
            width: 200px;
            height: 200px;
            top: 10%;
            left: 10%;
            animation-delay: 0s;
        }

        .shape-2 {
            width: 150px;
            height: 150px;
            top: 70%;
            right: 10%;
            animation-delay: 2s;
        }

        .shape-3 {
            width: 100px;
            height: 100px;
            top: 40%;
            left: 80%;
            animation-delay: 4s;
        }

        @keyframes float {
            0%, 100% {
                transform: translateY(0) rotate(0deg);
            }
            50% {
                transform: translateY(-20px) rotate(180deg);
            }
        }

        /* Responsive Design */
        @media (max-width: 480px) {
            .login-box {
                width: 90%;
                padding: 30px 20px;
                margin: 20px;
            }
            
            .login-header h2 {
                font-size: 24px;
            }
            
            .social-login {
                flex-direction: column;
            }
            
            .form-options {
                flex-direction: column;
                gap: 15px;
                align-items: flex-start;
            }
        }

        /* Loading Animation */
        .login-btn.loading {
            pointer-events: none;
        }

        .login-btn.loading span {
            opacity: 0;
        }

        .login-btn.loading::after {
            content: '';
            position: absolute;
            width: 20px;
            height: 20px;
            border: 2px solid rgba(255, 255, 255, 0.3);
            border-top: 2px solid white;
            border-radius: 50%;
            animation: spin 1s linear infinite;
        }

        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
    
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
    <div class="container">
        <div class="login-box">
            <div class="login-header">
                <div class="logo">
                    <i class="fas fa-user-circle"></i>
                </div>
                <h2>Chào Mừng Trở Lại</h2>
                <p>Đăng nhập vào tài khoản của bạn</p>
            </div>
            
            <form class="login-form" id="loginForm">
                <div class="input-group">
                    <div class="input-field">
                        <i class="fas fa-envelope"></i>
                        <input type="email" id="email" placeholder="Email" required>
                    </div>
                </div>
                
                <div class="input-group">
                    <div class="input-field">
                        <i class="fas fa-lock"></i>
                        <input type="password" id="password" placeholder="Mật khẩu" required>
                        <i class="fas fa-eye toggle-password" id="togglePassword"></i>
                    </div>
                </div>
                
                <div class="form-options">
                    <label class="remember-me">
                        <input type="checkbox" id="remember">
                        <span class="checkmark"></span>
                        Ghi nhớ đăng nhập
                    </label>
                    <a href="#" class="forgot-password">Quên mật khẩu?</a>
                </div>
                
                <button type="submit" class="login-btn">
                    <span>Đăng Nhập</span>
                    <i class="fas fa-arrow-right"></i>
                </button>
            </form>
            
            <div class="divider">
                <span>Hoặc đăng nhập với</span>
            </div>
            
            <div class="social-login">
                <button class="social-btn google">
                    <i class="fab fa-google"></i>
                    Google
                </button>
                <button class="social-btn facebook">
                    <i class="fab fa-facebook-f"></i>
                    Facebook
                </button>
            </div>
            
            <div class="signup-link">
                <p>Chưa có tài khoản? <a href="#">Đăng ký ngay</a></p>
            </div>
        </div>
        
        <div class="background-animation">
            <div class="shape shape-1"></div>
            <div class="shape shape-2"></div>
            <div class="shape shape-3"></div>
        </div>
    </div>
    
    <script>
        document.addEventListener('DOMContentLoaded', function() {
            const loginForm = document.getElementById('loginForm');
            const togglePassword = document.getElementById('togglePassword');
            const passwordInput = document.getElementById('password');
            const loginBtn = document.querySelector('.login-btn');
            
            // Toggle password visibility
            togglePassword.addEventListener('click', function() {
                const type = passwordInput.getAttribute('type') === 'password' ? 'text' : 'password';
                passwordInput.setAttribute('type', type);
                
                // Toggle eye icon
                this.classList.toggle('fa-eye');
                this.classList.toggle('fa-eye-slash');
            });
            
            // Form submission
            loginForm.addEventListener('submit', function(e) {
                e.preventDefault();
                
                const email = document.getElementById('email').value;
                const password = document.getElementById('password').value;
                
                // Validate inputs
                if (!email || !password) {
                    showMessage('Vui lòng điền đầy đủ thông tin!', 'error');
                    return;
                }
                
                if (!isValidEmail(email)) {
                    showMessage('Email không hợp lệ!', 'error');
                    return;
                }
                
                // Show loading state
                loginBtn.classList.add('loading');
                
                // Simulate API call
                setTimeout(() => {
                    loginBtn.classList.remove('loading');
                    
                    // Demo: Check credentials
                    if (email === 'demo@example.com' && password === '123456') {
                        showMessage('Đăng nhập thành công!', 'success');
                        // Redirect to dashboard
                        setTimeout(() => {
                            window.location.href = 'temperature.html';
                            // window.location.href = '#dashboard';
                        }, 1500);
                    } else {
                        showMessage('Email hoặc mật khẩu không chính xác!', 'error');
                    }
                }, 2000);
            });
            
            // Social login buttons
            document.querySelector('.social-btn.google').addEventListener('click', function() {
                showMessage('Đăng nhập với Google đang được phát triển...', 'info');
            });
            
            document.querySelector('.social-btn.facebook').addEventListener('click', function() {
                showMessage('Đăng nhập với Facebook đang được phát triển...', 'info');
            });
            
            // Input animations
            const inputs = document.querySelectorAll('.input-field input');
            inputs.forEach(input => {
                input.addEventListener('focus', function() {
                    this.parentElement.classList.add('focused');
                });
                
                input.addEventListener('blur', function() {
                    if (!this.value) {
                        this.parentElement.classList.remove('focused');
                    }
                });
            });
            
            // Utility functions
            function isValidEmail(email) {
                const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
                return emailRegex.test(email);
            }
            
            function showMessage(message, type) {
                // Remove existing messages
                const existingMessage = document.querySelector('.message');
                if (existingMessage) {
                    existingMessage.remove();
                }
                
                // Create message element
                const messageEl = document.createElement('div');
                messageEl.className = `message ${type}`;
                messageEl.textContent = message;
                
                // Add styles
                messageEl.style.cssText = `
                    position: fixed;
                    top: 20px;
                    right: 20px;
                    padding: 15px 20px;
                    border-radius: 8px;
                    color: white;
                    font-weight: 500;
                    z-index: 1000;
                    animation: slideIn 0.3s ease-out;
                    max-width: 300px;
                `;
                
                // Set background color based on type
                switch(type) {
                    case 'success':
                        messageEl.style.background = '#4CAF50';
                        break;
                    case 'error':
                        messageEl.style.background = '#f44336';
                        break;
                    case 'info':
                        messageEl.style.background = '#2196F3';
                        break;
                }
                
                // Add to page
                document.body.appendChild(messageEl);
                
                // Remove after 3 seconds
                setTimeout(() => {
                    messageEl.style.animation = 'slideOut 0.3s ease-out';
                    setTimeout(() => {
                        messageEl.remove();
                    }, 300);
                }, 3000);
            }
            
            // Add CSS for message animations
            const style = document.createElement('style');
            style.textContent = `
                @keyframes slideIn {
                    from {
                        opacity: 0;
                        transform: translateX(100%);
                    }
                    to {
                        opacity: 1;
                        transform: translateX(0);
                    }
                }
                
                @keyframes slideOut {
                    from {
                        opacity: 1;
                        transform: translateX(0);
                    }
                    to {
                        opacity: 0;
                        transform: translateX(100%);
                    }
                }
            `;
            document.head.appendChild(style);
        });
    </script>

</body>
</html>
)=====";
