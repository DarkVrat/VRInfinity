document.getElementById('service').addEventListener('change', function() {
            updateTimeOptions();
        });

        document.getElementById('datepicker').addEventListener('change', function() {
            updateTimeOptions();
        });

        function updateDateOptions() {
            let today = new Date();
            let maxDate = new Date();
            maxDate.setDate(today.getDate() + 14);
			
			if (today.getDay() === 1 || today.getDay() === 2 || today.getHours() >= 20) {
				today.setDate(today.getDate() + 1);
			}

            document.getElementById('datepicker').min = today.toISOString().split('T')[0];
            document.getElementById('datepicker').max = maxDate.toISOString().split('T')[0];
        }

        function updateTimeOptions() {
            let serviceId = document.getElementById('service').value;
            let selectedDay = document.getElementById('datepicker').value;
            let timeSelect = document.getElementById('time');

            if (serviceId && selectedDay) {
                fetch('/get-available-times', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({ serviceId: serviceId, date: selectedDay })
                })
                .then(response => response.json())
                .then(data => {
                    timeSelect.innerHTML = '';
                    if (data.times && data.times.length > 0) {
                        data.times.forEach(function(time) {
                            let option = document.createElement('option');
                            option.value = time;
                            option.textContent = time;
                            timeSelect.appendChild(option);
                        });
                    } else {
                        let defaultOption = document.createElement('option');
                        defaultOption.textContent = 'Нет доступного времени';
                        timeSelect.appendChild(defaultOption);
                    }
                })
                .catch(error => {
                    console.error('Ошибка:', error);
                });
            }
        }

        window.onload = function() {
            updateDateOptions();
        };