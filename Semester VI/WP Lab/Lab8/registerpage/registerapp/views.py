from django.shortcuts import render, redirect
from django.views.decorators.csrf import csrf_protect
from .forms import RegisterForm

@csrf_protect
def register(request):
    if request.method == "POST":
        form = RegisterForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data['username']
            email = form.cleaned_data['email']
            contact_number = form.cleaned_data['contact_number']
            
            # Securely pass data via session (protected by CSRF)
            request.session['username'] = username
            request.session['email'] = email
            request.session['contact_number'] = contact_number
            
            return redirect('success')
    else:
        form = RegisterForm()
    return render(request, 'register.html', {'form': form})

@csrf_protect
def success(request):
    username = request.session.get('username')
    email = request.session.get('email')
    contact_number = request.session.get('contact_number')
    
    return render(request, 'success.html', {
        'username': username,
        'email': email,
        'contact_number': contact_number
    })
