document.addEventListener("DOMContentLoaded", function() {
    const timeElements = document.querySelectorAll("timeForJS");

    const currentYear = new Date().getFullYear();

    timeElements.forEach(function(element) {
        const timestamp = element.textContent.trim();
        const date = new Date(timestamp);
        const elementYear = date.getFullYear();

        const options = {
            month: 'long',
            day: 'numeric',
            hour: '2-digit',
            minute: '2-digit',
        };
        if (elementYear !== currentYear) {
            options.year = 'numeric';
        }

        const formattedDate = new Intl.DateTimeFormat('ru-RU', options).format(date);

        element.textContent = formattedDate;
    });
});
