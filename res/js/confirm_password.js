document.getElementById('toggle-confirm-password').addEventListener('click', function () {
    var confirmPasswordField = document.getElementById('confirm-password');
    var confirmPasswordFieldType = confirmPasswordField.getAttribute('type');
    if (confirmPasswordFieldType === 'password') {
        confirmPasswordField.setAttribute('type', 'text');
        this.textContent = 'Скрыть';
    } else {
        confirmPasswordField.setAttribute('type', 'password');
        this.textContent = 'Показать';
    }
});

document.getElementById('register-form').addEventListener('submit', function (event) {
    var password = document.getElementById('password').value;
    var confirmPassword = document.getElementById('confirm-password').value;

    if (password !== confirmPassword) {
        event.preventDefault();
        alert('Пароли не совпадают. Пожалуйста, попробуйте снова.');
    }
});